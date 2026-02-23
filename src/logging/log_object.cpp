#include "log_object.hpp"

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
    const LogLevel&    _level,
    const LogCategory& _category,
    std::string        _message,
    std::string        _file,
    int                _line,
    std::string        _function
)
    : level{_level},
      category{_category},
      file{std::move(_file)},
      line{_line},
      function{std::move(_function)},
      message{std::move(_message)}
{
}