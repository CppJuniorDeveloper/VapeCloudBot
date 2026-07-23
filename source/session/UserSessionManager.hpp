#pragma once

#include <unordered_map>
#include <optional>
#include <mutex>

#include "UserSession.hpp"

namespace Session
{
    class SessionManager
    {
    public:

        static SessionManager& getInstnace() noexcept;

        bool empty() const noexcept;
        void update(Session::UserSession session) noexcept;
        void remove(int64_t id) noexcept;
        std::optional<Session::UserSession> get(int64_t id) const noexcept;

    private:

        SessionManager() = default;
        ~SessionManager() = default;
        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;
        SessionManager(SessionManager&&) = delete;
        SessionManager& operator=(SessionManager&&) = delete;

    private:

        std::unordered_map<int64_t, Session::UserSession> m_sessions;
        mutable std::mutex m_mtx;
    };
}
