#include "CommandHandler.hpp"

bool Handlers::CommandHandler::handle(Message message)
{
    auto it = m_commands.find(message->text);

    if (it == m_commands.end())
    {
        return false;
    }

    it->second(message);
    return true;
}

void Handlers::CommandHandler::addCommand(const std::string &command, CommandCallback command_callback)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_commands.find(command);

    if (it == m_commands.end())
    {
        m_commands[command] = std::move(command_callback);
    }
}
