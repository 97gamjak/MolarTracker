#ifndef __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__
#define __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__

#include "connections/connection.hpp"

namespace settings
{
    class LoggingSettings;   // Forward declaration
}

namespace ui
{
    class LoggingController
    {
       private:
        Connection _logLevelConnection;

       public:
        LoggingController() = delete;
        LoggingController(settings::LoggingSettings& loggingSettings);

       private:
        void subscribe(settings::LoggingSettings& loggingSettings);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__