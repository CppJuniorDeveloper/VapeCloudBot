#include "UserSessionManager.hpp"

Session::SessionManager &Session::SessionManager::getInstnace() noexcept
{
    static SessionManager instance;
    return instance;
}

bool Session::SessionManager::empty() const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    return m_sessions.empty();
}

void Session::SessionManager::update(UserSession session) noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

     m_sessions[session.userId] = std::move(session);
}

void Session::SessionManager::remove(int64_t id) noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    m_sessions.erase(id);
}

std::optional<Session::UserSession> Session::SessionManager::get(int64_t id) const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = m_sessions.find(id);
    if (it != m_sessions.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}
