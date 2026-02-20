#ifndef __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__
#define __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__

#include "connections/connection.hpp"

namespace settings
{
    class LoggingSettings;   // Forward declaration
}

namespace ui
{
    /**
     * @brief Controller for managing logging settings and behavior in the UI.
     *
     * This controller is responsible for subscribing to changes in the logging
     * settings and updating the logging behavior accordingly. It manages a
     * connection to the logging settings to ensure that any changes are
     * reflected in the application's logging system.
     *
     */
    class LoggingController
    {
       private:
        /// Connection object for the logging settings change subscription. This
        /// is used to manage the subscription and ensure that it can be
        /// properly disconnected when the controller is destroyed.
        Connection _logLevelConnection;

       public:
        LoggingController() = delete;
        LoggingController(settings::LoggingSettings& loggingSettings);

       private:
        void subscribe(settings::LoggingSettings& loggingSettings);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__LOGGING_CONTROLLER_HPP__