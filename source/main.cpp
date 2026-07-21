#include <tgbot/tgbot.h>
#include "logger/logger.hpp"
#include "../config/Config.hpp"

int main() {

    auto& logger = Logger::getInstance();
    logger.init(LOGGER_PATH);

    TgBot::Bot bot(TOKEN);
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onAnyMessage([&bot, &logger](TgBot::Message::Ptr message) {
        logger.info("User wrote " + message->text);
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });
    try {
        logger.info("Bot username: " + bot.getApi().getMe()->username);
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            logger.info("Long poll started");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& tg_bot_e)
    {
        logger.error("Telegram API error: " + std::string(tg_bot_e.what()));
    }
    catch (std::exception& e)
    {
        logger.error("Error: " + std::string(e.what()));
    }
    return 0;
}
