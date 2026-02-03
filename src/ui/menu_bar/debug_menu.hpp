#ifndef __UI__MENU_BAR__DEBUG_MENU_HPP__
#define __UI__MENU_BAR__DEBUG_MENU_HPP__

#include <QObject>

class QAction;    // Forward declaration
class QMenuBar;   // Forward declaration
class QMenu;      // Forward declaration

namespace ui
{
    class DebugMenu : public QObject
    {
        Q_OBJECT

       signals:
        void requestDebugSlots();
        void requestLogViewer();

       private:
        QMenu* _debugMenu;

        QAction* _debugSlotsAction = nullptr;
        QAction* _logViewerAction  = nullptr;

       public:
        explicit DebugMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__DEBUG_MENU_HPP__