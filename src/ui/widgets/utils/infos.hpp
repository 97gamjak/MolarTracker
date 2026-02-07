#ifndef __UI__WIDGETS__UTILS__INFOS_HPP__
#define __UI__WIDGETS__UTILS__INFOS_HPP__

#include <QStatusBar>

#include "logging/log_object.hpp"

namespace ui
{
    void showInfoStatusBar(LogObject logObject, QStatusBar* statusBar);
}

#endif   // __UI__WIDGETS__UTILS__INFOS_HPP__