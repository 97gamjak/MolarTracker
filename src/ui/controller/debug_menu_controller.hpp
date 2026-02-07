#ifndef __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>

#include "logging/logging_base.hpp"
#include "ui/widgets/logging/debug_slots_dialog.hpp"
#include "ui/widgets/logging/log_viewer_dialog.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}

class QMainWindow;   // Forward declaration

namespace ui
{
    class DebugMenu;          // Forward declaration
    class DebugSlotsDialog;   // Forward declaration
    class UndoStack;          // Forward declaration

    /**
     * @brief Controller for the debug menu actions
     *
     */
    class DebugMenuController : public QObject
    {
        Q_OBJECT

       private:
        QMainWindow&     _mainWindow;
        DebugMenu&       _debugMenu;
        app::AppContext& _appContext;
        UndoStack&       _undoStack;

        QPointer<DebugSlotsDialog> _debugSlotsDialog;
        QPointer<LogViewerDialog>  _logViewerDialog;
        LogViewerDialog::Settings  _logViewerSettings;

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
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__