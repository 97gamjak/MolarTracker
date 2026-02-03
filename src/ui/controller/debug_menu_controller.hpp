#ifndef __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__

#include <QObject>

namespace app
{
    class AppContext;   // Forward declaration
}

class QMainWindow;   // Forward declaration

namespace ui
{
    class DebugMenu;
    class DebugSlotsDialog;
    class LogViewerDialog;

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

        DebugSlotsDialog* _debugSlotsDialog = nullptr;
        LogViewerDialog*  _logViewerDialog  = nullptr;

       private slots:
        void _onRequestDebugSlots();
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
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__DEBUG_MENU_CONTROLLER_HPP__