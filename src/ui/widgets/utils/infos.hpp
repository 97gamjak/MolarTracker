#ifndef __UI__WIDGETS__UTILS__INFOS_HPP__
#define __UI__WIDGETS__UTILS__INFOS_HPP__

class QStatusBar;   // forward declaration

namespace logging
{
    class LogObject;   // forward declaration
}   // namespace logging

namespace ui
{
    void showInfoStatusBar(logging::LogObject logObject, QStatusBar* statusBar);

}   // namespace ui

#endif   // __UI__WIDGETS__UTILS__INFOS_HPP__