#ifndef __LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__LOG_MANAGER_HPP__

#include <string>
#include <unordered_map>

enum class LogLevel;
enum class LogCategory : size_t;

class LogManager
{
   private:
    std::unordered_map<LogCategory, LogLevel> _categories;

   public:
    static LogManager& getInstance();

    bool isEnabled(const LogCategory& category, const LogLevel& level) const;

    void setLogLevel(const LogCategory& category, const LogLevel& level);

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

    void initCategoryMap();
};

#endif   // __LOGGING__LOG_MANAGER_HPP__