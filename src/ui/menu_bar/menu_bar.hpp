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
    class DebugMenu;   // Forward declaration

    class MenuBar : public QMenuBar
    {
        Q_OBJECT

       private:
        FileMenu     _fileMenu;
        EditMenu     _editMenu;
        DebugMenu    _debugMenu;
        SettingsMenu _settingsMenu;
        HelpMenu     _helpMenu;

       public:
        explicit MenuBar(QWidget* parent = nullptr);

        FileMenu&     getFileMenu() { return _fileMenu; }
        EditMenu&     getEditMenu() { return _editMenu; }
        DebugMenu&    getDebugMenu() { return _debugMenu; }
        SettingsMenu& getSettingsMenu() { return _settingsMenu; }
        HelpMenu&     getHelpMenu() { return _helpMenu; }
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__MENU_BAR_HPP__