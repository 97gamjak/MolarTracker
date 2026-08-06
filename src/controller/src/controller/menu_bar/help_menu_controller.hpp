#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__

#include <QObject>
#include <memory>

class QMainWindow;   // Forward declaration

namespace ui
{
    class HelpMenu;   // Forward declaration
}   // namespace ui

namespace service
{
    class IMigrationLogService;   // Forward declaration
}   // namespace service

namespace controller
{
    /**
     * @brief Controller for the help menu actions
     *
     */
    class HelpMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the help menu
        ui::HelpMenu& _helpMenu;
        /// The migration log service, used to populate the migration
        /// history dialog
        std::shared_ptr<service::IMigrationLogService> _migrationLogService;

       private slots:
        void _onHelpPageRequested();
        void _onMigrationHistoryRequested();
        void _onAboutRequested();

       public:
        explicit HelpMenuController(
            QMainWindow&  mainWindow,
            ui::HelpMenu& helpMenu,
            const std::shared_ptr<service::IMigrationLogService>&
                migrationLogService
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__