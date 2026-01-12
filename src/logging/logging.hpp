#ifndef __LOGGING__LOGGING_HPP__
#define __LOGGING__LOGGING_HPP__

struct LogCategory
{
    const char* name;
};

class LogStream
{
   public:
    LogStream(
        LogLevel           level,
        const LogCategory& category,
        const char*        file,
        int                line,
        const char*        function
    )
        : _enabled{LogManager::instance().isEnabled(category, level)},
          _level{level},
          _category{category},
          _file{file},
          _line{line},
          _function{function}
    {
    }

    ~LogStream()
    {
        if (_enabled)
        {
            LogManager::instance().write(
                _level,
                _category,
                _file,
                _line,
                _function,
                std::move(_buffer)
            );
        }
    }

    template <typename T>
    LogStream& operator<<(const T& v)
    {
        if (_enabled)
        {
            _buffer +=
                QString::fromUtf8(QVariant::fromValue(v).toString().toUtf8());
        }
        return *this;
    }

   private:
    bool               _enabled;
    LogLevel           _level;
    const LogCategory& _category;
    const char*        _file;
    int                _line;
    const char*        _function;
    QString            _buffer;
};

class LogEntryScope
{
   public:
    LogEntryScope(
        LogLevel           level,
        const LogCategory& category,
        const char*        function,
        const char*        file,
        int                line
    )
        : _level{level},
          _category{category},
          _function{function},
          _file{file},
          _line{line},
          _enabled{LogManager::instance().isEnabled(category, level)}
    {
        if (_enabled)
        {
            LogManager::instance().write(
                _level,
                _category,
                _file,
                _line,
                _function,
                QStringLiteral("→ enter ") + _function
            );
        }
    }

    ~LogEntryScope()
    {
        if (_enabled)
        {
            LogManager::instance().write(
                _level,
                _category,
                _file,
                _line,
                _function,
                QStringLiteral("← exit  ") + _function
            );
        }
    }

   private:
    LogLevel           _level;
    const LogCategory& _category;
    const char*        _function;
    const char*        _file;
    int                _line;
    bool               _enabled;
};

class TimedLogEntryScope final : public LogEntryScope
{
   public:
    using Clock = std::chrono::steady_clock;

    TimedLogEntryScope(
        LogLevel           level,
        const LogCategory& category,
        const char*        function,
        const char*        file,
        int                line
    )
        : LogEntryScope(level, category, function, file, line),
          _start{Clock::now()}
    {
    }

    ~TimedLogEntryScope()
    {
        if (_enabled)
        {
            const auto ms =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    Clock::now() - _start
                )
                    .count();

            LogManager::instance().write(
                _level,
                _category,
                _file,
                _line,
                _function,
                QString("← exit %1 (%2 ms)").arg(_function).arg(ms)
            );
        }
    }

   private:
    Clock::time_point _start;
};

#pragma once
#include <array>
#include <string_view>

enum class LogCategoryId : uint16_t
{
#define LOG_CATEGORY(token, str) token,
#include "log_categories.def"
#undef LOG_CATEGORY
    _count
};

struct LogCategory
{
    LogCategoryId id;
    const char*   name;   // stable C string for display/rules
};

inline constexpr std::array<LogCategory, static_cast<size_t>(LogCategoryId::_count)>
kAllLogCategories = {{
#define LOG_CATEGORY(token, str) LogCategory{LogCategoryId::token, str},
#include "log_categories.def"
#undef LOG_CATEGORY
}};

inline constexpr const LogCategory& logCategory(LogCategoryId id) noexcept
{
    return kAllLogCategories[static_cast<size_t>(id)];
}

#endif   // __LOGGING__LOGGING_HPP__