#include <cstdlib>
#include <exception>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "exceptions/base.hpp"
#include "logging/log_manager.hpp"
#include "ui/application.hpp"
#include "ui/main_window.hpp"
#include "ui/widgets/exceptions/exception_dialog.hpp"

#define __LOG_CATEGORY__ LogCategory::application
#include "logging/log_macros.hpp"

// TODO: move this to some initialization module
// https://97gamjak.atlassian.net/browse/MOLTRACK-96
/**
 * @brief Initializes the global configuration
 *
 * @note Sets up logging and other global settings.
 */
void initGlobalConfig()
{
    // Initialize logging
    const auto dataPath = Constants::getInstance().getDataPath();
    LogManager::getInstance().initializeRingFileLogger(dataPath);
}

int main(int argc, char** argv)
{
    ui::MolarTrackerApplication app{argc, argv};

    try
    {
        initGlobalConfig();

        app::AppContext appContext;
        ui::MainWindow  mainWindow{appContext};

        mainWindow.show();

        return app.exec();
    }
    catch (const MolarTrackerException& e)
    {
        const auto& logObject = LOG_ERROR_OBJECT(e.what());
        ui::ExceptionDialog::showFatal("A fatal error occurred", logObject);
    }
    catch (const std::exception& e)
    {
        const auto& logObject = LOG_ERROR_OBJECT(e.what());
        ui::ExceptionDialog::showFatal(
            "An unexpected error occurred",
            logObject
        );
    }

    return EXIT_FAILURE;
}