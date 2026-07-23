#include "RegisterBot.hpp"
#include "../utils/messages/MessageTable.hpp"
#include "../utils/translation/TranslationClient.hpp"

#include <random>

std::string lang = "ru";

void Core::registerBot(Bot &bot, const std::string &token)
{
    bot.authorize(token);

    bot.addCommand("/start", [&](TgBot::Message::Ptr message)
    {
        std::string msg = Utils::MESSAGE_TABLE.at("onCommandStart");
        bot.api().sendMessage(message->chat->id, Utils::TranslationClient::translate(msg, lang));
    });

    bot.addCommand("/random_language", [&](TgBot::Message::Ptr message)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, Utils::TranslationClient::SUPPORTED_LANGUAGES.size() - 1);

        auto it = Utils::TranslationClient::SUPPORTED_LANGUAGES.begin();
        std::advance(it, dist(gen));
        lang = *it;
    });

    bot.addCommand("/get_language", [&](TgBot::Message::Ptr message)
    {
         bot.api().sendMessage(message->chat->id, lang);
    });
}
