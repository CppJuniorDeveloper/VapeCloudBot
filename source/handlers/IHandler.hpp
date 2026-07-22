#pragma once

#include <tgbot/tgbot.h>
#include <functional>
#include <unordered_map>

namespace Core
{
    template<typename Obj>
    class IHandler
    {
    protected:

        TgBot::Bot& m_bot_ref;

    public:

        explicit IHandler(TgBot::Bot& bot) : m_bot_ref(bot) {}
        virtual ~IHandler() = default;

    public:

        virtual bool handle(Obj data) = 0;
    };

}
