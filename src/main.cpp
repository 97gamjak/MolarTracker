#include <QApplication>
#include <exception>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "exceptions/base.hpp"
#include "ui/exceptions/exception_dialog.hpp"
#include "ui/main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    app.setApplicationName(Constants::APP_NAME);
    app.setApplicationDisplayName(Constants::APP_NAME);

    app.setDesktopFileName(Constants::DESKTOP_APP_NAME);

    try
    {
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