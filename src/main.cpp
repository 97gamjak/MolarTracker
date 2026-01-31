#include <cstdlib>
#include <exception>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "exceptions/base.hpp"
#include "logging/log_manager.hpp"
#include "ui/application.hpp"
#include "ui/exceptions/exception_dialog.hpp"
#include "ui/main_window.hpp"

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
    const auto configDir = Constants::getInstance().getConfigPath();
    LogManager::getInstance().initializeRingFileLogger(configDir);
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
        ui::ExceptionDialog::showFatal("A fatal error occurred", e.what());
    }
    catch (const std::exception& e)
    {
        ui::ExceptionDialog::showFatal(
            "An unexpected error occurred",
            e.what()
        );
    }

    return EXIT_FAILURE;
}