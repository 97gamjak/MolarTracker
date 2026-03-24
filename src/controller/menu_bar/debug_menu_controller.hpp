#ifndef __CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__
#define __CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>

#include "ui/widgets/logging/debug_slots_dialog.hpp"
#include "ui/widgets/logging/log_viewer_dialog.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace logging
{
    class LogCategories;   // Forward declaration
}   // namespace logging

class QMainWindow;   // Forward declaration

namespace ui
{
    class DebugMenu;   // Forward declaration
    class UndoStack;   // Forward declaration
}   // namespace ui

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
        ui::UndoStack& _undoStack;
        /// Settings for the log viewer dialog
        std::shared_ptr<ui::LogViewerDialog::Settings> _logViewerSettings;

        /// Pointer to the debug slots dialog, this is a QPointer to ensure that
        /// we do not have a dangling pointer if the dialog is closed outside of
        /// this controller
        QPointer<ui::DebugSlotsDialog> _debugSlotsDialog;

        /// Pointer to the log viewer dialog, this is a QPointer to ensure that
        /// we do not have a dangling pointer if the dialog is closed outside of
        /// this controller
        QPointer<ui::LogViewerDialog> _logViewerDialog;

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
            ui::UndoStack&   undoStack
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

#endif   // __CONTROLLER__MENU_BAR__DEBUG_MENU_CONTROLLER_HPP__