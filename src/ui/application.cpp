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
        setApplicationName(Constants::APP_NAME);
        setApplicationDisplayName(Constants::APP_NAME);

        setDesktopFileName(Constants::DESKTOP_APP_NAME);
    }

}   // namespace ui