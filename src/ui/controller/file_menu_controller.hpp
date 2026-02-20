#ifndef __UI__CONTROLLER__FILE_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__FILE_MENU_CONTROLLER_HPP__

#include <QObject>

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

class QMainWindow;   // Forward declaration

namespace ui
{
    class FileMenu;   // Forward declaration

    /**
     * @brief Binds the file menu actions to their respective functionalities.
     *
     */
    class FileMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the file menu
        FileMenu& _fileMenu;
        /// Reference to the application context
        app::AppContext& _appContext;

       private slots:
        void _onRequestSave();
        void _onRequestQuit();

       public:
        explicit FileMenuController(
            QMainWindow&     mainWindow,
            FileMenu&        fileMenu,
            app::AppContext& appContext
        );
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__FILE_MENU_CONTROLLER_HPP__