#include "logging_handler.hpp"

#include "logging/log_manager.hpp"
#include "settings/logging_settings.hpp"

namespace ui
{
    /**
     * @brief Construct a new Logging Handler:: Logging Handler object
     *
     * @param loggingSettings
     */
    LoggingHandler::LoggingHandler(settings::LoggingSettings& loggingSettings)
    {
        // we handle here the subscription directly in the constructor, as we
        // know that the log manager instance is global
        subscribe(loggingSettings);
    }

    /**
     * @brief Subscribe to changes in logging settings
     *
     * This needs to be done in a separate handler for a clean RAII handling
     * destruction of the connection. Otherwise, we would need to avoid
     * LogManager being a global singleton and instead pass it to the Settings
     * class and then to the UI, which would be a much bigger refactor.
     *
     * @param loggingSettings
     */
    void LoggingHandler::subscribe(settings::LoggingSettings& loggingSettings)
    {
        auto& param         = loggingSettings.getDefaultLogLevelParam();
        _logLevelConnection = param.subscribe(
            [](void* user, const LogLevel& newLogLevel)
            {
                if (auto* logManager = static_cast<LogManager*>(user))
                    logManager->setDefaultLogLevel(newLogLevel);
            },
            &LogManager::getInstance()
        );
    }

}   // namespace ui