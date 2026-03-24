#include <cstdlib>
#include <exception>

#include "controller/main_controller.hpp"
#include "exceptions/base.hpp"
#include "logging/log_macros.hpp"
#include "logging/log_manager.hpp"
#include "ui/application.hpp"
#include "ui/widgets/exceptions/exception_dialog.hpp"

REGISTER_LOG_CATEGORY("Application");

int main(int argc, char** argv)
{
    ui::MolarTrackerApplication app{argc, argv};

    try
    {
        controller::MainController mainController;
        mainController.start();

        return ui::MolarTrackerApplication::exec();
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