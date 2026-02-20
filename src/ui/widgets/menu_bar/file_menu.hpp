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
        /// Signal emitted when the user requests to save
        void requestSave();
        /// Signal emitted when the user requests to quit
        void requestQuit();

       private:
        /// Pointer to the file menu widget
        QMenu* _fileMenu = nullptr;

        /// The save action in the file menu
        QAction* _saveAction = nullptr;
        /// The quit action in the file menu
        QAction* _quitAction = nullptr;

       public:
        explicit FileMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__FILE_MENU_HPP__