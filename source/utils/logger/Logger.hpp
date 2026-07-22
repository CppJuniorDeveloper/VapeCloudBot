#pragma once

#include <string>
#include <mutex>
#include <fstream>

namespace Utils
{
    class Logger
    {
    public:

        enum class Level
        {
            DEBUG = 0,
            INFO = 1,
            WARNING = 2,
            ERR = 3
        };

        void init(const std::string& filepath);

        void setMinLevel(Level log_level) noexcept;
        Level getMinLevel() const noexcept;

        void log(const std::string& log_message, Level log_level, const std::string& tag = "");

        void debug(const std::string& log_message, const std::string& tag = "");
        void info(const std::string& log_message, const std::string& tag = "");
        void warning(const std::string& log_message, const std::string& tag = "");
        void error(const std::string& log_message, const std::string& tag = "");

        static Logger& getInstance();

    private:

        Logger() = default;
        ~Logger();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    private:

        std::string m_levelToString(Level log_level) const noexcept;
        std::string m_currentTime() const noexcept;
        void m_checkRotation();
        void m_writeData(const std::string& log_data);

        Level m_minimumLogLevel = Level::DEBUG;
        std::string m_filepath{};
        bool m_initialized = false;
        mutable std::mutex m_mtx;
        std::ofstream m_file;
    };
}
