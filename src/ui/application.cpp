#include "ui/application.hpp"

#include "config/constants.hpp"

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
        const auto desktopAppName =
            QString::fromStdString(Constants::getDesktopAppName());

        setApplicationName(appName);
        setApplicationDisplayName(appName);

        setDesktopFileName(desktopAppName);
    }

}   // namespace ui