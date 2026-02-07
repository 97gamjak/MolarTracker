#ifndef __UI__WIDGETS__UTILS__WARNINGS_HPP__
#define __UI__WIDGETS__UTILS__WARNINGS_HPP__

#include <QMessageBox>
#include <string>

#include "logging/log_object.hpp"

namespace ui
{
    void showWarningMessageBox(const std::string& title, LogObject logObject);
}

#endif   // __UI__WIDGETS__UTILS__WARNINGS_HPP__