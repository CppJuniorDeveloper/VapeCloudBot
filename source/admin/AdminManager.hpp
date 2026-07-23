#pragma once

#include <vector>
#include <string>
#include <mutex>

namespace Admin
{
    class AdminManager
    {
    public:

        static AdminManager& getInstance() noexcept;

        bool loadFromJson(const std::string& path);
        bool saveToJson(const std::string& path) const;

        bool isAdmin(int64_t id) const noexcept;
        bool empty() const noexcept;

        void add(int64_t id) noexcept;
        void remove(int64_t id) noexcept;

        std::size_t adminsCount() const noexcept;

    private:

        AdminManager() = default;
        ~AdminManager() = default;

        AdminManager(const AdminManager&) = delete;
        AdminManager& operator=(const AdminManager&) = delete;
        AdminManager(AdminManager&&) = delete;
        AdminManager& operator=(AdminManager&&) = delete;

    private:

        std::vector<int64_t> m_admins;
        mutable std::mutex m_mtx;
    };
}
