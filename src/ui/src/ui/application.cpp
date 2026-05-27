#include "ui/application.hpp"

#include "config/constants.hpp"
#include "logging/log_macros.hpp"
#include "ui/exceptions/exception_dialog.hpp"

REGISTER_LOG_CATEGORY("UI.Application");

namespace ui
{
    /**
     * @brief Constructs a new Molar Tracker Application
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    MolarTrackerApplication::MolarTrackerApplication(int& argc, char** argv)
        : QApplication(argc, argv)
    {
        const auto appName = QString::fromStdString(Constants::getAppName());
        const auto version = QString::fromStdString(Constants::getVersion());
        const auto desktopAppName =
            QString::fromStdString(Constants::getDesktopAppName());
        const auto appDisplayName =
            QString::fromStdString(Constants::getAppDisplayName());

        setApplicationName(appName);
        setApplicationDisplayName(appDisplayName);
        setApplicationVersion(version);

        setDesktopFileName(desktopAppName);
    }

    /**
     * @brief Handles unhandled exceptions
     *
     * @param receiver The object receiving the event
     * @param event The event being processed
     * @return true if the event was handled, false otherwise
     */
    bool MolarTrackerApplication::notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (const std::exception& e)
        {
            handleException(e.what());
        }
        return false;
    }

    /**
     * @brief Handles exceptions thrown within the application
     *
     * @param what The exception message
     */
    void MolarTrackerApplication::handleException(const char* what)
    {
        const auto& logObject = LOG_ERROR_OBJECT(what);
        ui::ExceptionDialog::showFatal(
            "An unexpected error occurred",
            logObject
        );
    }

}   // namespace ui