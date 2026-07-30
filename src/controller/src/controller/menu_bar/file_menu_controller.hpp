#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__FILE_MENU_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__FILE_MENU_CONTROLLER_HPP__

#include <QObject>

namespace store
{
    class StoreContainer;   // Forward declaration
}   // namespace store

namespace ui
{
    class MainWindow;   // Forward declaration
    class FileMenu;     // Forward declaration
}   // namespace ui

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace controller
{
    /**
     * @brief Binds the file menu actions to their respective functionalities.
     *
     */
    class FileMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        ui::MainWindow& _mainWindow;
        /// Reference to the file menu
        ui::FileMenu& _fileMenu;
        /// Reference to the store container
        store::StoreContainer& _storeContainer;
        /// Reference to the application settings
        settings::Settings& _settings;

       private slots:
        void _onRequestSave();
        void _onRequestQuit();

       public:
        explicit FileMenuController(
            ui::MainWindow&        mainWindow,
            ui::FileMenu&          fileMenu,
            store::StoreContainer& storeContainer,
            settings::Settings&    settings
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__FILE_MENU_CONTROLLER_HPP__