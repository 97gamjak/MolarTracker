#include "logging/log_object.hpp"

namespace logging
{
    /**
     * @brief Construct a new Log Object:: Log Object object
     *
     * @param _level
     * @param _category
     * @param _message
     * @param _file
     * @param _line
     * @param _function
     */
    LogObject::LogObject(
        const LogLevel& _level,
        std::string     _category,
        std::string     _message,
        std::string     _file,
        int             _line,
        std::string     _function
    )
        : level{_level},
          category{std::move(_category)},
          file{std::move(_file)},
          line{_line},
          function{std::move(_function)},
          message{std::move(_message)}
    {
    }

}   // namespace logging
