#ifndef __LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__LOG_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <unordered_map>

#include "utils/ring_file.hpp"

enum class LogLevel : size_t;      // forward declaration
enum class LogCategory : size_t;   // forward declaration

struct LogObject;   // forward declaration

namespace settings
{
    class LoggingSettings;   // forward declaration
}

/**
 * @brief Singleton class managing logging categories and levels
 *
 */
class LogManager
{
   private:
    std::unordered_map<LogCategory, LogLevel> _categories{};
    RingFile                                  _ringFile{};
    std::filesystem::path                     _logDirectory{};

   public:
    static LogManager& getInstance();

    void initializeRingFileLogger(
        const settings::LoggingSettings& settings,
        const std::filesystem::path&     directory
    );
    void changeLogLevel(const LogCategory& category, const LogLevel& level);
    bool isEnabled(const LogCategory& category, const LogLevel& level) const;
    void flush();

    std::unordered_map<LogCategory, LogLevel> getCategories() const;
    std::unordered_map<LogCategory, LogLevel> getDefaultCategories() const;

    std::filesystem::path getCurrentLogFilePath() const;

    void log(const LogObject& logObject);

   private:
    LogManager();

    std::string _logLevelToString(const LogLevel& level) const;
};

#endif   // __LOGGING__LOG_MANAGER_HPP__