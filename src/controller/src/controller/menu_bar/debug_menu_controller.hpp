#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace logging
{
    class LogCategories;   // Forward declaration
}   // namespace logging

namespace ui
{
    class DebugMenu;          // Forward declaration
    class DebugSlotsDialog;   // Forward declaration
    class LogViewerDialog;    // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace controller
{

    /**
     * @brief Controller for the debug menu actions
     *
     */
    class DebugMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// References to the main window
        QMainWindow& _mainWindow;
        /// Reference to the debug menu
        ui::DebugMenu& _debugMenu;
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Reference to the undo stack for executing commands
        cmd::UndoStack& _undoStack;
        /// Settings for the log viewer dialog
        std::shared_ptr<ui::LogViewerDialog::Settings> _logViewerSettings;

        /// Pointer to the debug slots dialog
        ui::DebugSlotsDialog* _debugSlotsDialog = nullptr;
        /// Pointer to the log viewer dialog
        ui::LogViewerDialog* _logViewerDialog = nullptr;

       private slots:
        void _onRequestDebugSlots();
        void _onDebugSlotsChangeRequested(
            const ui::DebugSlotsDialog::Action& action,
            const logging::LogCategories&       categories
        );
        void _onRequestLogViewer();

       public:
        explicit DebugMenuController(
            QMainWindow&     mainWindow,
            ui::DebugMenu&   debugMenu,
            app::AppContext& appContext,
            cmd::UndoStack&  undoStack
        );

       private:
        void _ensureDebugSlotsDialog();
        void _ensureLogViewerDialog();
        void _resetDefaultDebugFlags();
        void _applyDebugFlagChanges(const logging::LogCategories& categories);
        void _applyDebugFlagChangesAndClose(
            const logging::LogCategories& categories
        );
        void _applyLogViewerSettings();
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__