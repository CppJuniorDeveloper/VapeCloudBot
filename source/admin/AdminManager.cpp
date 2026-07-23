#include "AdminManager.hpp"

#include "nlohmann/json.hpp"
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

Admin::AdminManager &Admin::AdminManager::getInstance() noexcept
{
    static AdminManager instance;
    return instance;
}

bool Admin::AdminManager::loadFromJson(const std::string &path)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    std::ifstream json_file(path);
    if (!json_file.is_open())
    {
        return false;
    }

    json data;
    json_file >> data;

    try
    {
        auto admins = data["Admins"].get<std::vector<int64_t>>();
        m_admins.reserve(admins.size());
        for (auto id : admins)
        {
            m_admins.push_back(id);
        }
        json_file.close();
        return true;
    }
    catch(const json::exception& e)
    {
        json_file.close();
        return false;
    }
}

bool Admin::AdminManager::saveToJson(const std::string &path) const
{
    std::lock_guard<std::mutex> lock(m_mtx);

    std::ofstream json_file(path, std::ios::trunc);
    if (!json_file.is_open())
    {
        return false;
    }

    json data;
    data["Admins"] = m_admins;

    json_file << data.dump(4);
    json_file.close();
    return true;
}

bool Admin::AdminManager::isAdmin(int64_t id) const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_admins.empty())
    {
        return false;
    }

    for (auto admin_id : m_admins)
    {
        if (id == admin_id)
        {
            return true;
        }
    }
    return false;
}

bool Admin::AdminManager::empty() const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    return m_admins.empty();
}

void Admin::AdminManager::add(int64_t id) noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = std::find(m_admins.begin(), m_admins.end(), id);
    if (it == m_admins.end())
    {
        return;
    }
    m_admins.push_back(id);
}

void Admin::AdminManager::remove(int64_t id) noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_admins.empty())
    {
        return;
    }

    auto it = std::find(m_admins.begin(), m_admins.end(), id);
    if (it != m_admins.end())
    {
        m_admins.erase(it);
    }
}

std::size_t Admin::AdminManager::adminsCount() const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);

    return m_admins.size();
}
