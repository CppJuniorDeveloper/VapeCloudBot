#include "Logger.hpp"

#include <ctime>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void Logger::init(const std::string &filepath)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_initialized)
    {
        return;
    }

    m_filepath = filepath;
    m_file.open(m_filepath, std::ios::app);
    if (!m_file.is_open())
    {
        throw std::runtime_error("Failed to open log file");
    }
    m_initialized = true;
}

void Logger::setMinLevel(Level log_level) noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_minimumLogLevel = log_level;
}

Logger::Level Logger::getMinLevel() const noexcept
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_minimumLogLevel;
}

void Logger::log(const std::string &log_message, Level log_level, const std::string &tag)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (!m_initialized)
    {
        std::cerr << "Logger not initialized! Call init() first." << std::endl;
        return;
    }

    if (static_cast<int>(log_level) >= static_cast<int>(m_minimumLogLevel))
    {
        std::string log = "[" + m_currentTime() + "] [" + m_levelToString(log_level) + "] ";

        if (!tag.empty())
        {
            log += "[" + tag + "] ";
        }

        log += log_message;

        std::cout << log << std::endl;
        m_writeData(log);
    }
}

void Logger::debug(const std::string &log_message, const std::string &tag)
{
    log(log_message, Level::DEBUG, tag);
}

void Logger::info(const std::string &log_message, const std::string &tag)
{
    log(log_message, Level::INFO, tag);
}

void Logger::warning(const std::string &log_message, const std::string &tag)
{
    log(log_message, Level::WARNING, tag);
}

void Logger::error(const std::string &log_message, const std::string &tag)
{
    log(log_message, Level::ERR, tag);
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

std::string Logger::m_levelToString(Level log_level) const noexcept
{
    switch (log_level)
    {
        case Level::DEBUG:   return "DEBUG";
        case Level::INFO:    return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERR:     return "ERROR";
        default:             return "UNKNOWN";
    }
}

std::string Logger::m_currentTime() const noexcept
{
    std::time_t now = std::time(nullptr);
    std::tm local_time{};

    #ifdef _WIN32
        localtime_s(&local_time, &now);
    #else
        localtime_r(&now, &local_time);
    #endif

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);

    return std::string(buffer);
}

void Logger::m_checkRotation()
{
    if (!m_initialized)
    {
        return;
    }

    static constexpr std::size_t MAX_LOG_FILE_SIZE = 10 * 1024 * 1024;

    if (!m_file.is_open()) return;

    std::streampos pos = m_file.tellp();
    if (pos < MAX_LOG_FILE_SIZE) return;

    m_file.close();

    std::string oldName = m_filepath + "." + std::to_string(std::time(nullptr)) + ".old";

    try
    {
        if (fs::exists(oldName)) {
            fs::remove(oldName);
        }
        fs::rename(m_filepath, oldName);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Log rotation error: " << e.what() << std::endl;
    }

    m_file.open(m_filepath, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to reopen log file");
    }
}

void Logger::m_writeData(const std::string &log_data)
{
    if (!m_initialized)
    {
        std::cerr << "LOGGER ERROR (not initialized): " << log_data << std::endl;
        return;
    }

    if (m_file.is_open())
    {
        m_checkRotation();
        m_file << log_data + '\n' << std::flush;
    }
    else
    {
         std::cerr << "LOGGER ERROR: " << log_data << std::endl;
    }
}
