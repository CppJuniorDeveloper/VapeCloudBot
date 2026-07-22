#pragma once

#include <tgbot/tgbot.h>
#include "../utils/logger/Logger.hpp"
#include "../handlers/CommandHandler.hpp"

#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <array>
#include <thread>
#include <atomic>

namespace Core
{
    class Bot
    {
    private:

        using Upd = TgBot::Update::Ptr;

        static inline constexpr unsigned int THREAD_COUNT    = 8u;
        static inline constexpr unsigned int UPDATES_TIMEOUT = 30u;
        static inline constexpr unsigned int UPDATES_LIMIT   = 100u;

        std::unique_ptr<TgBot::Bot> m_bot;

        std::unique_ptr<CommandHandler> m_commandHandler;

        Utils::Logger& m_logger = Utils::Logger::getInstance();

        std::mutex m_queueMtx;
        std::queue<Upd> m_updateQueue;
        std::array<std::thread, THREAD_COUNT> m_threads;
        std::thread m_fetcher;
        std::condition_variable m_queueCv;

        std::atomic<bool> m_running{false};
        bool m_authorized{false};
        int64_t m_lastUpdateId{0};

    private:

        Bot() = default;
        ~Bot();

    public:

        static Bot& getInstance();
        void authorize(const std::string& token);
        const TgBot::Api& api() const;

        void addCommand(const std::string& command, CommandHandler::CommandCallback callback);

        void start();
        void stop();
        bool isRunning();

    private:

        void checkAutorized() const;
        void m_joinAll();
        void m_fetchUpdates();
        void m_processUpdates();
        void m_handleUpdate(Upd update);
        void m_pushUpdate(Upd update);
        Upd m_popUpdate();

    };
}
