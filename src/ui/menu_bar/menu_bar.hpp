#ifndef __UI__MENU_BAR__MENU_BAR_HPP__
#define __UI__MENU_BAR__MENU_BAR_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration
class QMenuBar;      // Forward declaration
class QAction;       // Forward declaration

namespace ui
{
    class DebugMenu;   // Forward declaration

    class MenuBar : public QObject
    {
        Q_OBJECT

       private:
        QMainWindow& _mainWindow;

        DebugMenu* _debugMenu = nullptr;

        QAction* _undoAction        = nullptr;
        QAction* _redoAction        = nullptr;
        QAction* _saveAction        = nullptr;
        QAction* _quitAction        = nullptr;
        QAction* _preferencesAction = nullptr;
        QAction* _aboutAction       = nullptr;

       private:
        void _buildFileMenu(QMenuBar* menu);
        void _buildEditMenu(QMenuBar* menu);
        void _buildSettingsMenu(QMenuBar* menu);
        void _buildDebugMenu(QMenuBar* menu);
        void _buildHelpMenu(QMenuBar* menu);

       signals:
        void requestUndo();
        void requestRedo();
        void requestQuit();
        void requestPreferences();
        void requestDebug();
        void requestAbout();
        void requestSave();

       public:
        explicit MenuBar(QMainWindow& mainWindow);

        void build();

        void setUndoEnabled(bool enabled);
        void setRedoEnabled(bool enabled);

        void setUndoText(const QString& text);
        void setRedoText(const QString& text);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__MENU_BAR_HPP__