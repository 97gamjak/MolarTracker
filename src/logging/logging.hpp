#ifndef __LOGGING__LOGGING_HPP__
#define __LOGGING__LOGGING_HPP__

enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
    Off
};

struct LogCategory
{
    const char* name;
};

class LogManager
{
   public:
    static LogManager& getInstance();

    bool isEnabled(const LogCategory& category, LogLevel level) const;

    void log(
        LogLevel           level,
        const LogCategory& category,
        const char*        file,
        int                line,
        const char*        function,
        const std::string& message
    );
};

#endif   // __LOGGING__LOGGING_HPP__