#include <QApplication>
#include <cstdlib>
#include <exception>

#include "app/app_context.hpp"
#include "exceptions/base.hpp"
#include "ui/application.hpp"
#include "ui/exceptions/exception_dialog.hpp"
#include "ui/main_window.hpp"

int main(int argc, char** argv)
{
    ui::MolarTrackerApplication app{argc, argv};

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