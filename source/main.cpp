
#include "utils/logger/logger.hpp"
#include "../config/Config.hpp"
#include "core/TgBot.hpp"
#include "core/RegisterBot.hpp"

int main() {

    auto& logger = Utils::Logger::getInstance();
    logger.init(LOGGER_PATH);

    auto& bot = Core::Bot::getInstance();

    Core::registerBot(bot, TOKEN);

    try {
        //logger.info("Bot username: " + bot.getApi().getMe()->username);
        bot.start();
        while (bot.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
