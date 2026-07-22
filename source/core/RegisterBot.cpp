#include "RegisterBot.hpp"
#include "../utils/messages/MessageTable.hpp"

void Core::registerBot(Bot &bot, const std::string &token)
{
    bot.authorize(token);

    bot.addCommand("/start", [&](TgBot::Message::Ptr message)
    {
        bot.api().sendMessage(message->chat->id, Utils::MESSAGE_TABLE.at("onCommandStart"));
    });
}
