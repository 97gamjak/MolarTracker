#ifndef __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>

#include "ui/widgets/logging/debug_slots_dialog.hpp"
#include "ui/widgets/logging/log_viewer_dialog.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}

class QMainWindow;   // Forward declaration

namespace ui
{
    class DebugMenu;   // Forward declaration
    class UndoStack;   // Forward declaration

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
        DebugMenu& _debugMenu;
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Reference to the undo stack for executing commands
        UndoStack& _undoStack;
        /// Settings for the log viewer dialog
        LogViewerDialog::Settings _logViewerSettings;

        /// Pointer to the debug slots dialog, this is a QPointer to ensure that
        /// we do not have a dangling pointer if the dialog is closed outside of
        /// this controller
        QPointer<DebugSlotsDialog> _debugSlotsDialog;

        /// Pointer to the log viewer dialog, this is a QPointer to ensure that
        /// we do not have a dangling pointer if the dialog is closed outside of
        /// this controller
        QPointer<LogViewerDialog> _logViewerDialog;

       private slots:
        void _onRequestDebugSlots();
        void _onDebugSlotsChangeRequested(
            const DebugSlotsDialog::Action& action,
            const LogCategoryMap&           categories
        );
        void _onRequestLogViewer();

       public:
        explicit DebugMenuController(
            QMainWindow&     mainWindow,
            DebugMenu&       debugMenu,
            app::AppContext& appContext,
            UndoStack&       undoStack
        );

       private:
        void _ensureDebugSlotsDialog();
        void _ensureLogViewerDialog();
        void _resetDefaultDebugFlags();
        void _applyDebugFlagChanges(const LogCategoryMap& categories);
        void _applyDebugFlagChangesAndClose(const LogCategoryMap& categories);
        void _applyLogViewerSettings();
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__