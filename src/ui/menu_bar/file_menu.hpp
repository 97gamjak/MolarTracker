#ifndef __UI__MENU_BAR__FILE_MENU_HPP__
#define __UI__MENU_BAR__FILE_MENU_HPP__

#include <QObject>

class QAction;    // Forward declaration
class QMenu;      // Forward declaration
class QMenuBar;   // Forward declaration

namespace ui
{
    /**
     * @brief File menu in the menu bar
     *
     */
    class FileMenu : public QObject
    {
        Q_OBJECT

       signals:
        void requestSave();
        void requestQuit();

       private:
        QMenu* _fileMenu = nullptr;

        QAction* _saveAction = nullptr;
        QAction* _quitAction = nullptr;

       public:
        explicit FileMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__FILE_MENU_HPP__