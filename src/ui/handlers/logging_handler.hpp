#ifndef __UI__HANDLERS__LOGGING_HANDLER_HPP__
#define __UI__HANDLERS__LOGGING_HANDLER_HPP__

#include "connections/connection.hpp"

namespace settings
{
    class LoggingSettings;   // Forward declaration
}   // namespace settings

namespace ui
{
    /**
     * @brief Handler for managing logging settings and behavior in the UI.
     *
     * This handler is responsible for subscribing to changes in the logging
     * settings and updating the logging behavior accordingly. It manages a
     * connection to the logging settings to ensure that any changes are
     * reflected in the application's logging system.
     *
     */
    class LoggingHandler
    {
       private:
        /// Connection object for the logging settings change subscription. This
        /// is used to manage the subscription and ensure that it can be
        /// properly disconnected when the handler is destroyed.
        Connection _logLevelConnection;

       public:
        LoggingHandler() = delete;
        explicit LoggingHandler(settings::LoggingSettings& loggingSettings);

       private:
        void subscribe(settings::LoggingSettings& loggingSettings);
    };

}   // namespace ui

#endif   // __UI__HANDLERS__LOGGING_HANDLER_HPP__