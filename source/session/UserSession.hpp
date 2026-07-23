#pragma once

#include <string>

namespace Session
{
    enum class UserState
    {
        IDLE,
        ENTER_LANGUAGE
    };

    struct UserSession
    {
        int64_t userId;
        UserState state = UserState::IDLE;
        std::string languageCode;

        bool adminRoots = false;
    };
}
