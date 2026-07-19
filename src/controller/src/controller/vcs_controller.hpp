#ifndef __CONTROLLER__SRC__CONTROLLER__VCS_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__VCS_CONTROLLER_HPP__

#include <memory>

namespace vcs
{
    class UpdateCheckService;   // Forward declaration
}   // namespace vcs

namespace ui
{
    class MainWindow;   // Forward declaration
}   // namespace ui

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace controller
{
    /**
     * @brief Controller responsible for handling interactions with version
     * control systems, specifically for checking for updates on GitHub and
     * notifying the user about new releases. This controller will manage the
     * update check service and handle the logic for when to check for updates,
     * as well as how to notify the user and store their preferences regarding
     * update notifications.
     *
     */
    class VCSController
    {
       private:
        /// service that checks GitHub for newer releases
        std::unique_ptr<vcs::UpdateCheckService> _updateCheckService;

        /// The main window of the application
        std::shared_ptr<ui::MainWindow> _mainWindow;

        /// The settings for the application, used to store the dismissed update
        /// version
        std::shared_ptr<settings::Settings> _settings;

       public:
        explicit VCSController(
            const std::shared_ptr<ui::MainWindow>&     mainWindow,
            const std::shared_ptr<settings::Settings>& settings
        );
        ~VCSController();

        void start();
    };
}   //  namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__VCS_CONTROLLER_HPP__