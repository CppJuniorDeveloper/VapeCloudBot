
#include "utils/logger/logger.hpp"
#include "utils/translation/TranslationClient.hpp"
#include "../config/Config.hpp"
#include "core/TgBot.hpp"
#include "core/RegisterBot.hpp"
#include "admin/AdminManager.hpp"



namespace
{
    class Cleaner {
    private:

        Admin::AdminManager& m_adminManager;
        Utils::Logger& m_logger;

    public:
        Cleaner(Admin::AdminManager& admin_manager, Utils::Logger& logger)
            : m_adminManager(admin_manager), m_logger(logger) {}
        ~Cleaner() {
            bool success_load_to_json = m_adminManager.saveToJson(ADMIN_JSON_PATH);
            if (!success_load_to_json)
            {
                m_logger.warning("Failed to save admins to JSON.");
            }
            curl_global_cleanup();
        }
    };
}






int main() {

    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto& logger = Utils::Logger::getInstance();
    logger.init(LOGGER_PATH);

    auto& adminManager = Admin::AdminManager::getInstance();
    bool success_load_from_json = adminManager.loadFromJson(ADMIN_JSON_PATH);
    if (!success_load_from_json)
    {
        logger.warning("Failed to load admins from JSON.");
    }

    Cleaner cleaner{adminManager, logger};

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

    return 0;
}
