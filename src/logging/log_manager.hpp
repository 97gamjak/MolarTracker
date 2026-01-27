#ifndef __LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__LOG_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <unordered_map>

#include "utils/ring_file.hpp"

enum class LogLevel : size_t;
enum class LogCategory : size_t;

class LogManager
{
   private:
    std::unordered_map<LogCategory, LogLevel> _categories{};
    RingFile                                  _ringFile{};
    std::filesystem::path                     _logDirectory{};

   public:
    static LogManager& getInstance();

    void initializeRingFileLogger(const std::filesystem::path& configDir);
    void changeLogLevel(const LogCategory& category, const LogLevel& level);
    bool isEnabled(const LogCategory& category, const LogLevel& level) const;

    std::unordered_map<LogCategory, LogLevel> getCategories() const;
    std::unordered_map<LogCategory, LogLevel> getDefaultCategories() const;

    void log(
        const LogLevel&    level,
        const LogCategory& category,
        const std::string& file,
        const int          line,
        const std::string& function,
        const std::string& message
    );

   private:
    LogManager();

    std::string _logLevelToString(const LogLevel& level) const;
};

#endif   // __LOGGING__LOG_MANAGER_HPP__