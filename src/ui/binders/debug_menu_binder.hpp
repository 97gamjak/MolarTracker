#ifndef __UI__BINDERS__DEBUG_MENU_BINDER_HPP__
#define __UI__BINDERS__DEBUG_MENU_BINDER_HPP__

#include <QObject>

namespace ui
{
    class MenuBar;            // Forward declaration
    class MainWindow;         // Forward declaration
    class DebugSlotsDialog;   // Forward declaration

    class DebugMenuBinder : public QObject
    {
        Q_OBJECT

       private:
        MainWindow& _mainWindow;
        MenuBar&    _menuBar;

        DebugSlotsDialog* _dialog = nullptr;

       private slots:
        void _onRequestDebugSlots();

       public:
        explicit DebugMenuBinder(MainWindow& mainWindow, MenuBar& debugMenu);
    };

}   // namespace ui

#endif   // __UI__BINDERS__DEBUG_MENU_BINDER_HPP__