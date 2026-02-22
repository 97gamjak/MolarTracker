#include <cstdlib>
#include <exception>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "exceptions/base.hpp"
#include "logging/log_manager.hpp"
#include "settings/settings.hpp"
#include "ui/application.hpp"
#include "ui/controller/controllers.hpp"
#include "ui/main_window.hpp"
#include "ui/widgets/exceptions/exception_dialog.hpp"

#define __LOG_CATEGORY__ LogCategory::application
#include "logging/log_macros.hpp"

int main(int argc, char** argv)
{
    ui::MolarTrackerApplication app{argc, argv};

    try
    {
        // TODO: move this to some initialization module
        // https://97gamjak.atlassian.net/browse/MOLTRACK-96

        // initialize settings
        settings::Settings settings{Constants::getInstance().getConfigPath()};
        auto&              loggingSettings = settings.getLoggingSettings();

        // initialize ring file buffered logger
        LogManager::getInstance().initializeRingFileLogger(
            loggingSettings,
            Constants::getInstance().getDataPath()
        );

        app::AppContext appContext{settings};
        ui::Controllers controllers{settings};
        ui::MainWindow  mainWindow{appContext, controllers};

        mainWindow.show();
        mainWindow.start();

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