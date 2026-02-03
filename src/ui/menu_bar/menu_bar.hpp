#ifndef __UI__MENU_BAR__MENU_BAR_HPP__
#define __UI__MENU_BAR__MENU_BAR_HPP__

#include <QMenuBar>
#include <QObject>

#include "debug_menu.hpp"
#include "file_menu.hpp"
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
        DebugMenu    _debugMenu;
        SettingsMenu _settingsMenu;

        QAction* _undoAction  = nullptr;
        QAction* _redoAction  = nullptr;
        QAction* _aboutAction = nullptr;

       private:
        void _buildEditMenu(QMenuBar* menu);
        void _buildHelpMenu(QMenuBar* menu);

       signals:
        // edit menu
        void requestUndo();
        void requestRedo();
        // settings menu
        void requestPreferences();
        // help menu
        void requestAbout();

       public:
        explicit MenuBar(QWidget* parent = nullptr);

        FileMenu&     getFileMenu() { return _fileMenu; }
        DebugMenu&    getDebugMenu() { return _debugMenu; }
        SettingsMenu& getSettingsMenu() { return _settingsMenu; }

        void build();

        void setUndoEnabled(bool enabled);
        void setRedoEnabled(bool enabled);

        void setUndoText(const QString& text);
        void setRedoText(const QString& text);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__MENU_BAR_HPP__