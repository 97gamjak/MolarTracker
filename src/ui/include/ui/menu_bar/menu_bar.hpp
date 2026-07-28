#ifndef __UI__INCLUDE__UI__MENU_BAR__MENU_BAR_HPP__
#define __UI__INCLUDE__UI__MENU_BAR__MENU_BAR_HPP__

#include <QMenuBar>
#include <QMetaType>

#include "debug_menu.hpp"
#include "edit_menu.hpp"
#include "file_menu.hpp"
#include "help_menu.hpp"
#include "settings_menu.hpp"
#include "ui/menu_bar/utilities_menu.hpp"

class QWidget;   // Forward declaration

namespace settings
{
    class ShortcutSettings;   // Forward declaration
}   // namespace settings

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
        /// Utilities menu
        UtilitiesMenu _utilitiesMenu;

       public:
        explicit MenuBar(
            QWidget*                          parent,
            QMainWindow*                      mainWindow,
            const settings::ShortcutSettings& shortcutSettings
        );

        FileMenu&      getFileMenu();
        EditMenu&      getEditMenu();
        DebugMenu&     getDebugMenu();
        SettingsMenu&  getSettingsMenu();
        HelpMenu&      getHelpMenu();
        UtilitiesMenu& getUtilitiesMenu();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MENU_BAR__MENU_BAR_HPP__