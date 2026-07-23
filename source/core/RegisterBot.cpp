#include "RegisterBot.hpp"

#include "../utils/logger/Logger.hpp"
#include "../utils/messages/MessageTable.hpp"
#include "../utils/translation/TranslationClient.hpp"
#include "../utils/keyboard/KeyboardBuilder.hpp"
#include "../session/UserSessionManager.hpp"

#include <future>

void Core::registerBot(Bot &bot, const std::string &token)
{
    bot.authorize(token);

    auto& manager = Session::SessionManager::getInstnace();
    auto& logger  = Utils::Logger::getInstance();

    bot.addCommand("/start", [&](TgBot::Message::Ptr message)
    {
        auto session = manager.get(message->from->id);
        if (!session.has_value())
        {
            session = Session::UserSession {
                message->from->id,
                Session::UserState::IDLE,
                message->from->languageCode
            };
        }
        session->state = Session::UserState::IDLE;

        std::string msg = Utils::COMMAND_MESSAGE_TABLE.at("onCommandStart");
        bot.api().sendMessage(message->chat->id, Utils::TranslationClient::translate(msg, session->languageCode));

        manager.update(*session);
    });

    bot.addCommand("/language", [&](TgBot::Message::Ptr message)
    {
        auto session = manager.get(message->from->id);
        if (!session.has_value())
        {
            session = Session::UserSession {
                message->from->id,
                Session::UserState::IDLE,
                message->from->languageCode
            };
        }
        session->state = Session::UserState::IDLE;

        std::string lang = session->languageCode;

        auto futureButton = std::async(std::launch::async, [&]() {
            return Utils::TranslationClient::translate(
                Utils::BUTTON_LABEL_TABLE.at("onButtonChooseOwnLanguageCode"),
                lang
            );
        });

        auto futureMessage = std::async(std::launch::async, [&]() {
            return Utils::TranslationClient::translate(
                Utils::COMMAND_MESSAGE_TABLE.at("onCommandLanguage"),
                lang
            );
        });

        std::string translatedButton = futureButton.get();
        std::string translatedMessage = futureMessage.get();

        auto keyboard = Utils::KeyboardBuilder::InlineBuilder()
            .addButton("🇷🇺 Русский (ru)", "lang_ru")
            .addButton("🇺🇦 Українська (uk)", "lang_uk")
            .newRow()
            .addButton("🇰🇿 Қазақша (kk)", "lang_kk")
            .addButton("🇧🇾 Беларуская (be)", "lang_be")
            .newRow()
            .addButton("🇹🇷 Türkçe (tr)", "lang_tr")
            .addButton("🇺🇿 Oʻzbekcha (uz)", "lang_uz")
            .newRow()
            .addButton("🇬🇧 English (en)", "lang_en")
            .addButton("🇩🇪 Deutsch (de)", "lang_de")
            .newRow()
            .addButton("🇫🇷 Français (fr)", "lang_fr")
            .addButton("🇪🇸 Español (es)", "lang_es")
            .newRow()
            .addButton(translatedButton, "lang_other")
            .build();

        bot.api().sendMessage(
            message->chat->id,
            translatedMessage,
            nullptr,
            nullptr,
            keyboard
        );

        manager.update(*session);
    });

    bot.addCommand("/admin", [&](TgBot::Message::Ptr message)
    {
        auto session = manager.get(message->from->id);
        if (!session.has_value())
        {
            session = Session::UserSession {
                message->from->id,
                Session::UserState::IDLE,
                message->from->languageCode
            };
        }
        session->state = Session::UserState::IDLE;

        if (session->adminRoots)
        {

            auto futureAdminMessage = std::async(std::launch::async, [&]()
            {
                return Utils::TranslationClient::translate(Utils::COMMAND_MESSAGE_TABLE.at("onCommandAdmin"));
            });
            auto futureAdminButton1 = std::async(std::launch::async, [&]()
            {
                return Utils::TranslationClient::translate(Utils::BUTTON_LABEL_TABLE.at("onButtonEnableTranslation"));
            });
            auto futureAdminButton2 = std::async(std::launch::async, [&]()
            {
                return Utils::TranslationClient::translate(Utils::BUTTON_LABEL_TABLE.at("onButtonDisableTranslation"));
            });

            std::string admin_message = futureAdminMessage.get() + " " + std::to_string(session->userId);
            std::string admin_button1 = futureAdminButton1.get();
            std::string admin_button2 = futureAdminButton2.get();

            auto keyboard = Utils::KeyboardBuilder::InlineBuilder()
                .addButton(admin_button1, "admin_enable_translation")
                .newRow()
                .addButton(admin_button2, "admin_disable_translation")
                .build();

            bot.api().sendMessage(
                message->chat->id, admin_message, nullptr, nullptr, keyboard
            );
        }
        else
        {
            bot.api().sendMessage(
                message->chat->id,
                Utils::TranslationClient::translate(Utils::ERROR_MESSAGE_TABLE.at("onDenyAdminRoots"))
            );
        }

        manager.update(*session);
    });
}
