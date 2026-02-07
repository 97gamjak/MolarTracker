#ifndef __LOGGING__LOG_OBJECT_HPP__
#define __LOGGING__LOG_OBJECT_HPP__

#include <string>

#include "logging_base.hpp"

/**
 * @brief Struct representing a log message and its metadata
 *
 */
struct LogObject
{
    LogLevel    level;
    LogCategory category;
    std::string file;
    int         line;
    std::string function;
    std::string message;

    LogObject(
        const LogLevel&    _level,
        const LogCategory& _category,
        const std::string& _message,
        const std::string& _file,
        int                _line,
        const std::string& _function
    )
        : level{_level},
          category{_category},
          file{_file},
          line{_line},
          function{_function},
          message{_message}
    {
    }
};

#endif   // __LOGGING__LOG_OBJECT_HPP__