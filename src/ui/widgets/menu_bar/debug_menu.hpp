#ifndef __UI__MENU_BAR__DEBUG_MENU_HPP__
#define __UI__MENU_BAR__DEBUG_MENU_HPP__

#include <QObject>

class QAction;    // Forward declaration
class QMenuBar;   // Forward declaration
class QMenu;      // Forward declaration

namespace ui
{
    /**
     * @brief Debug menu in the menu bar
     *
     */
    class DebugMenu : public QObject
    {
        Q_OBJECT

       private:
        /// Pointer to the debug menu in the menu bar
        QMenu* _debugMenu = nullptr;

        /// Pointer to the action for opening the debug slots dialog
        QAction* _debugSlotsAction = nullptr;
        /// Pointer to the action for opening the log viewer dialog
        QAction* _logViewerAction = nullptr;

       public:
        explicit DebugMenu(QMenuBar& menuBar);

       signals:
        /// QT signal for when the debug slots action is triggered
        void requestDebugSlots();
        /// QT signal for when the log viewer action is triggered
        void requestLogViewer();
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__DEBUG_MENU_HPP__