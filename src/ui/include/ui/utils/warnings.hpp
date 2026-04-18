#ifndef __UI__INCLUDE__UI__UTILS__WARNINGS_HPP__
#define __UI__INCLUDE__UI__UTILS__WARNINGS_HPP__

#include <string>

namespace logging
{
    struct LogObject;   // forward declaration
}   // namespace logging

class QWidget;   // forward declaration

namespace ui
{
    void showWarningMessageBox(
        const std::string& title,
        logging::LogObject logObject,
        QWidget*           parent
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__UTILS__WARNINGS_HPP__