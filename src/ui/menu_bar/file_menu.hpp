#ifndef __UI__MENU_BAR__FILE_MENU_HPP__
#define __UI__MENU_BAR__FILE_MENU_HPP__

#include <QObject>

class QAction;       // Forward declaration
class QMainWindow;   // Forward declaration
class QMenu;         // Forward declaration

namespace ui
{
    class FileMenu : public QObject
    {
        Q_OBJECT

       signals:
        void requestSave();
        void requestQuit();

       private:
        QMainWindow& _mainWindow;

        QAction* _saveAction = nullptr;
        QAction* _quitAction = nullptr;

       public:
        explicit FileMenu(QMainWindow& mainWindow);

        void build(QMenu* parentMenu);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__FILE_MENU_HPP__