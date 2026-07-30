#ifndef __UI__INCLUDE__UI__MENU_BAR__UTILITIES_MENU_HPP__
#define __UI__INCLUDE__UI__MENU_BAR__UTILITIES_MENU_HPP__

#include <QObject>

class QMenu;         // Forward declaration
class QMenuBar;      // Forward declaration
class QAction;       // Forward declaration
class QMainWindow;   // Forward declaration

class ShortcutSet;   // Forward declaration

namespace ui
{
    /**
     * @brief Utilities menu in the menu bar
     *
     */
    class UtilitiesMenu : public QObject
    {
        Q_OBJECT

       private:
        /// Pointer to the help menu widget
        QMenu* _utilitiesMenu = nullptr;

        /// The action to open the help page
        QAction* _screenshotAction = nullptr;

        /// The main window to capture for screenshots
        QMainWindow* _captureRoot = nullptr;

       public:
        explicit UtilitiesMenu(
            QMenuBar&          menuBar,
            QMainWindow*       captureRoot,
            const ShortcutSet& screenshotShortcut
        );

        void requestScreenshot();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MENU_BAR__UTILITIES_MENU_HPP__