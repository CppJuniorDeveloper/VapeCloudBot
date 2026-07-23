#pragma once

#include "IHandler.hpp"

namespace Handlers
{
    class CommandHandler : public IHandler<TgBot::Message::Ptr>
    {
    public:

        using Message = TgBot::Message::Ptr;
        using CommandCallback = std::function<void(Message)>;

        explicit CommandHandler(TgBot::Bot& bot) : IHandler(bot) {}

    public:

        virtual bool handle(Message message) override;
        void addCommand(const std::string& command, CommandCallback command_callback);

    private:

        std::unordered_map<std::string, CommandCallback> m_commands;
        std::mutex m_mtx;

    };
}
