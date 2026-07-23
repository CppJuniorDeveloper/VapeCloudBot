
#include "utils/logger/logger.hpp"
#include "utils/translation/TranslationClient.hpp"
#include "../config/Config.hpp"
#include "core/TgBot.hpp"
#include "core/RegisterBot.hpp"

int main() {

    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto& logger = Utils::Logger::getInstance();
    logger.init(LOGGER_PATH);

    auto& bot = Core::Bot::getInstance();

    Core::registerBot(bot, TOKEN);

    try {
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

    curl_global_cleanup();

    return 0;
}
