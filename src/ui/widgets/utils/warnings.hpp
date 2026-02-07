#ifndef __UI__WIDGETS__UTILS__WARNINGS_HPP__
#define __UI__WIDGETS__UTILS__WARNINGS_HPP__

#include <QMessageBox>

class LogObject;

namespace ui
{
    void showWarningMessageBox(const std::string& title, LogObject logObject);
}

#endif   // __UI__WIDGETS__UTILS__WARNINGS_HPP__