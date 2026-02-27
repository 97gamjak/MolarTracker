#ifndef __LOGGING__LOG_OBJECT_HPP__
#define __LOGGING__LOG_OBJECT_HPP__

#include <string>

#include "config/logging_base.hpp"
#include "log_category.hpp"

namespace logging
{
    class LogCategory;

    /**
     * @brief Struct representing a log message and its metadata
     *
     */
    struct LogObject
    {
       public:   // members
        /// The log level of the message (e.g., Debug, Info, Warning, Error)
        LogLevel level;

        /// The log category of the message (e.g., application, ui, database,
        /// etc.)
        LogCategory category;

        /// The source file where the log message was generated
        std::string file;

        /// The line number in the source file where the log message was
        /// generated
        int line;

        /// The function name where the log message was generated
        std::string function;

        /// The actual log message content
        std::string message;

       public:   // methods
        LogObject(
            const LogLevel&    _level,
            const LogCategory& _category,
            std::string        _message,
            std::string        _file,
            int                _line,
            std::string        _function
        );
    };

}   // namespace logging

#endif   // __LOGGING__LOG_OBJECT_HPP__