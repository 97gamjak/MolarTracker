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