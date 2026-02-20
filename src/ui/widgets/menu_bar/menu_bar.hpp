#ifndef __UI__MENU_BAR__MENU_BAR_HPP__
#define __UI__MENU_BAR__MENU_BAR_HPP__

#include <QMenuBar>
#include <QObject>

#include "debug_menu.hpp"
#include "edit_menu.hpp"
#include "file_menu.hpp"
#include "help_menu.hpp"
#include "settings_menu.hpp"

class QMainWindow;   // Forward declaration
class QAction;       // Forward declaration

namespace ui
{
    /**
     * @brief Application menu bar inheriting from QMenuBar
     *
     */
    class MenuBar : public QMenuBar
    {
        Q_OBJECT

       private:
        /// File menu
        FileMenu _fileMenu;
        /// Edit menu
        EditMenu _editMenu;
        /// Debug menu
        DebugMenu _debugMenu;
        /// Settings menu
        SettingsMenu _settingsMenu;
        /// Help menu
        HelpMenu _helpMenu;

       public:
        explicit MenuBar(QWidget* parent = nullptr);

        FileMenu&     getFileMenu();
        EditMenu&     getEditMenu();
        DebugMenu&    getDebugMenu();
        SettingsMenu& getSettingsMenu();
        HelpMenu&     getHelpMenu();
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__MENU_BAR_HPP__