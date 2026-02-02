#ifndef __UI__MENU_BAR__MENU_BAR_HPP__
#define __UI__MENU_BAR__MENU_BAR_HPP__

#include <QMenuBar>
#include <QObject>

#include "file_menu.hpp"

class QMainWindow;   // Forward declaration
class QAction;       // Forward declaration

namespace ui
{
    class DebugMenu;   // Forward declaration

    class MenuBar : public QMenuBar
    {
        Q_OBJECT

       private:
        DebugMenu* _debugMenu = nullptr;
        FileMenu   _fileMenu;

        QAction* _undoAction        = nullptr;
        QAction* _redoAction        = nullptr;
        QAction* _preferencesAction = nullptr;
        QAction* _aboutAction       = nullptr;

       private:
        void _buildFileMenu(QMenuBar* menu);
        void _buildEditMenu(QMenuBar* menu);
        void _buildSettingsMenu(QMenuBar* menu);
        void _buildDebugMenu(QMenuBar* menu);
        void _buildHelpMenu(QMenuBar* menu);

       signals:
        // edit menu
        void requestUndo();
        void requestRedo();
        // file menu
        void requestQuit();
        void requestSave();
        // debug menu
        void requestDebugSlots();
        void requestLogViewer();
        // settings menu
        void requestPreferences();
        // help menu
        void requestAbout();

       public:
        explicit MenuBar(QWidget* parent = nullptr);

        FileMenu& getFileMenu() { return _fileMenu; }

        void build();

        void setUndoEnabled(bool enabled);
        void setRedoEnabled(bool enabled);

        void setUndoText(const QString& text);
        void setRedoText(const QString& text);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__MENU_BAR_HPP__