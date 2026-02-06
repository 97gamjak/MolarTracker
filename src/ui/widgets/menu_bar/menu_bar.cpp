#include "menu_bar.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

#include "debug_menu.hpp"
#include "file_menu.hpp"

namespace ui
{

    /**
     * @brief Construct a new Menu Bar:: Menu Bar object
     *
     * @param parent
     */
    MenuBar::MenuBar(QWidget* parent)
        : QMenuBar{parent},
          _fileMenu{*this},
          _editMenu{*this},
          _debugMenu{*this},
          _settingsMenu{*this},
          _helpMenu{*this}
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

}   // namespace ui