#ifndef __UI__BINDERS__DEBUG_MENU_BINDER_HPP__
#define __UI__BINDERS__DEBUG_MENU_BINDER_HPP__

#include <QObject>

namespace ui
{
    class MenuBar;            // Forward declaration
    class MainWindow;         // Forward declaration
    class DebugSlotsDialog;   // Forward declaration
    class LogViewerDialog;    // Forward declaration

    class DebugMenuBinder : public QObject
    {
        Q_OBJECT

       private:
        MainWindow& _mainWindow;
        MenuBar&    _menuBar;

        DebugSlotsDialog* _debugSlotDialog = nullptr;
        LogViewerDialog*  _logViewerDialog = nullptr;

       private slots:
        void _onRequestDebugSlots();
        void _onRequestLogViewer();

       public:
        explicit DebugMenuBinder(MainWindow& mainWindow, MenuBar& menuBar);

       private:
        void _ensureDebugSlotDialog();
        void _ensureLogViewerDialog();
    };

}   // namespace ui

#endif   // __UI__BINDERS__DEBUG_MENU_BINDER_HPP__