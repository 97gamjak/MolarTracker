#include "ui/menu_bar/menu_bar.hpp"

#include <QMenuBar>

#include "settings/shortcut_settings.hpp"

class QWidget;   // Forward declaration

namespace ui
{

    class DebugMenu;      // Forward declaration
    class FileMenu;       // Forward declaration
    class EditMenu;       // Forward declaration
    class SettingsMenu;   // Forward declaration
    class HelpMenu;       // Forward declaration

    /**
     * @brief Construct a new Menu Bar:: Menu Bar object
     *
     * @param parent
     * @param mainWindow The main window to which the menu bar belongs
     * @param shortcutSettings The shortcut settings to use for the menu bar
     */
    MenuBar::MenuBar(
        QWidget*                          parent,
        QMainWindow*                      mainWindow,
        const settings::ShortcutSettings& shortcutSettings
    )
        : QMenuBar{parent},
          _fileMenu{
              *this,
              shortcutSettings.getSaveShortcut(),
              shortcutSettings.getQuitShortcut()
          },
          _editMenu{*this},
          _debugMenu{*this},
          _settingsMenu{*this},
          _helpMenu{*this},
          _utilitiesMenu{
              *this,
              mainWindow,
              shortcutSettings.getScreenshotShortcut()
          }
    {
    }

    /**
     * @brief Get the File Menu object
     *
     * @return FileMenu&
     */
    FileMenu& MenuBar::getFileMenu() { return _fileMenu; }

    /**
     * @brief Get the Edit Menu object
     *
     * @return EditMenu&
     */
    EditMenu& MenuBar::getEditMenu() { return _editMenu; }

    /**
     * @brief Get the Debug Menu object
     *
     * @return DebugMenu&
     */
    DebugMenu& MenuBar::getDebugMenu() { return _debugMenu; }

    /**
     * @brief Get the Settings Menu object
     *
     * @return SettingsMenu&
     */
    SettingsMenu& MenuBar::getSettingsMenu() { return _settingsMenu; }

    /**
     * @brief Get the Help Menu object
     *
     * @return HelpMenu&
     */
    HelpMenu& MenuBar::getHelpMenu() { return _helpMenu; }

    /**
     * @brief Get the Utilities Menu object
     *
     * @return UtilitiesMenu&
     */
    UtilitiesMenu& MenuBar::getUtilitiesMenu() { return _utilitiesMenu; }

}   // namespace ui
