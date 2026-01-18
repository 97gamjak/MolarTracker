#ifndef __LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__LOG_MANAGER_HPP__

#include <string>

class LogLevel;
enum class LogCategoryId;
class LogCategory;

class LogManager
{
   public:
    static LogManager& getInstance();

    bool isEnabled(const LogCategory& category, LogLevel level) const;

    void registerBuiltInCategories();
    void registerCategory(LogCategoryId id, std::string_view name);

    void log(
        LogLevel           level,
        const LogCategory& category,
        const char*        file,
        int                line,
        const char*        function,
        const std::string& message
    );
};

#endif   // __LOGGING__LOG_MANAGER_HPP__