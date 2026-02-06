#ifndef __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>

#include "logging/logging_base.hpp"
#include "ui/logging/debug_slots_dialog.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}

class QMainWindow;   // Forward declaration

namespace ui
{
    class DebugMenu;          // Forward declaration
    class DebugSlotsDialog;   // Forward declaration
    class LogViewerDialog;    // Forward declaration

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

        QPointer<DebugSlotsDialog> _debugSlotsDialog;
        QPointer<LogViewerDialog>  _logViewerDialog;

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
            app::AppContext& appContext
        );

       private:
        void _ensureDebugSlotsDialog();
        void _ensureLogViewerDialog();
        void _resetDefaultDebugFlags();
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__