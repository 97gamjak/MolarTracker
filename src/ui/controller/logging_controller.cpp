#include "logging_controller.hpp"

#include "logging/log_manager.hpp"
#include "settings/logging_settings.hpp"

namespace ui
{
    /**
     * @brief Construct a new Logging Controller:: Logging Controller object
     *
     * @param loggingSettings
     */
    LoggingController::LoggingController(
        settings::LoggingSettings& loggingSettings
    )
    {
        subscribe(loggingSettings);
    }

    /**
     * @brief Subscribe to changes in logging settings
     *
     * This needs to be done in a separate controller for a clean RAII handling
     * destruction of the connection. Otherwise, we would need to avoid
     * LogManager being a global singleton and instead pass it to the Settings
     * class and then to the UI, which would be a much bigger refactor.
     *
     * @param loggingSettings
     */
    void LoggingController::subscribe(
        settings::LoggingSettings& loggingSettings
    )
    {
        auto& param         = loggingSettings.getDefaultLogLevelParam();
        _logLevelConnection = param.subscribe(
            [](void* user, const LogLevel& newLogLevel)
            {
                // TODO(97gamjak): this should be done via a command and
                // undo stack
                // https://97gamjak.atlassian.net/browse/MOLTRACK-102
                if (auto* logManager = static_cast<LogManager*>(user))
                    logManager->setDefaultLogLevel(newLogLevel);
            },
            &LogManager::getInstance()
        );
    }

}   // namespace ui