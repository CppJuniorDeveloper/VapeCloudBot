#include "TgBot.hpp"

Core::Bot::~Bot()
{
    m_running.store(false);
    m_queueCv.notify_all();
    m_joinAll();
}

Core::Bot &Core::Bot::getInstance()
{
    static Bot instance;
    return instance;
}

void Core::Bot::authorize(const std::string &token)
{
    if (m_authorized)
    {
        m_logger.warning("was made an attempt to re-authorize the bot", "BOT");
        return;
    }

    m_bot = std::make_unique<TgBot::Bot>(token);

    m_commandHandler = std::make_unique<Handlers::CommandHandler>(*m_bot);

    m_authorized = true;
}

const TgBot::Api &Core::Bot::api() const
{
    checkAutorized();
    return m_bot->getApi();
}

void Core::Bot::addCommand(const std::string &command, Handlers::CommandHandler::CommandCallback callback)
{
    checkAutorized();
    m_commandHandler->addCommand(command, callback);
}

void Core::Bot::start()
{
    checkAutorized();
    if (m_running.load())
    {
        m_logger.warning("Bot already running", "BOT");
        return;
    }

    m_logger.info("Started the bot", "BOT");
    m_running.store(true);

    m_fetcher = std::thread(&Core::Bot::m_fetchUpdates, this);

    for (std::size_t i = 0; i < THREAD_COUNT; ++i)
    {
        m_threads[i] = std::thread(&Core::Bot::m_processUpdates, this);
    }

     m_logger.info("Bot started with " + std::to_string(THREAD_COUNT) + " workers", "BOT");
}

void Core::Bot::stop()
{
    if (!m_running.load())
    {
        return;
    }
    m_logger.info("Stopping bot...", "BOT");

    m_running.store(false);
    m_queueCv.notify_all();
    m_joinAll();

    m_logger.info("Bot stopped", "BOT");
}

bool Core::Bot::isRunning()
{
    return m_running.load();
}

void Core::Bot::checkAutorized() const
{
    if (!m_authorized)
    {
        m_logger.error("Bot authorization failed: invalid token or token expired", "BOT");
        throw TgBot::TgException("Authorization failed. Please check your bot token.", TgBot::TgException::ErrorCode::Unauthorized);
    }
}

void Core::Bot::m_joinAll()
{
    for (auto& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    if (m_fetcher.joinable())
    {
        m_fetcher.join();
    }
}

void Core::Bot::m_fetchUpdates()
{
    m_logger.info("Fetcher thread started", "BOT");

    while (m_running.load())
    {
        try
        {
            auto updates = m_bot->getApi().getUpdates(
                m_lastUpdateId + 1,
                UPDATES_LIMIT,
                UPDATES_TIMEOUT,
                {}
            );

            if (!updates.empty())
            {
                for (const auto& update : updates)
                {
                    m_pushUpdate(update);
                    if (update->updateId > m_lastUpdateId)
                    {
                        m_lastUpdateId = update->updateId;
                    }
                }
                m_queueCv.notify_all();
                m_logger.debug("Fetched " + std::to_string(updates.size()) + " updates", "BOT");
            }
        }
        catch (const TgBot::TgException& e)
        {
            m_logger.error("Telegram API error: " + std::string(e.what()), "BOT");
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        catch (const std::exception& e)
        {
            m_logger.error("Fetcher error: " + std::string(e.what()), "BOT");
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    m_logger.info("Fetcher thread stopped", "BOT");
}

void Core::Bot::m_processUpdates()
{
    while (m_running.load())
    {
        std::unique_lock<std::mutex> lock(m_queueMtx);
        m_queueCv.wait(lock, [this] () -> bool
        {
            return !m_updateQueue.empty() || !m_running.load();
        });
        auto update = m_popUpdate();
        lock.unlock();

        if (update)
        {
            m_handleUpdate(std::move(update));
        }
    }
}

void Core::Bot::m_handleUpdate(Upd update)
{
    m_logger.debug("Received update (ID: " + std::to_string(update->updateId) + ")", "BOT");

     if (update->message)
        {
            auto msg = update->message;
            std::string chatId = std::to_string(msg->chat->id);
            std::string username = !msg->from->username.empty() ? msg->from->username : "unknown";

            if ((!msg->text.empty()) && (msg->text.find("/") == 0))
            {
                m_logger.info("[COMMAND] from @" + username + " (chat: " + chatId + "): " + msg->text, "BOT");
                m_commandHandler->handle(msg);
            }
            else
            {
                m_logger.info("[MESSAGE] from @" + username + " (chat: " + chatId + "): " + msg->text, "BOT");
                //m_messageHandler->handleMessage(msg);
            }
            return;
        }

        if (update->callbackQuery)
        {
            //m_callbackHandler->handleCallback(update->callbackQuery);
            return;
        }

        if (update->inlineQuery)
        {
            //m_inlineHandler->handleInlineQuery(update->inlineQuery);
            return;
        }
}

void Core::Bot::m_pushUpdate(Upd update)
{
    std::lock_guard<std::mutex> lock(m_queueMtx);
    m_updateQueue.push(update);
}

Core::Bot::Upd Core::Bot::m_popUpdate()
{
    if (m_updateQueue.empty()) {
        return nullptr;
    }
    auto update = std::move(m_updateQueue.front());
    m_updateQueue.pop();
    return update;
}
