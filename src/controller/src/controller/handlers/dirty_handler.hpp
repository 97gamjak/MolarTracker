#ifndef __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__

#include "connections/connection.hpp"

namespace store
{
    class StoreContainer;   // Forward declaration
}   // namespace store

namespace ui
{
    class MainWindow;   // Forward declaration
}   // namespace ui

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace controller
{
    /**
     * @brief Handler for managing the dirty state of the application, this
     * handler is responsible for tracking the dirty state of the application
     * and providing methods for marking the application as dirty or clean, it
     * can also be used to subscribe to changes in the dirty state and to notify
     * subscribers when the dirty state changes.
     *
     */
    class DirtyStateHandler
    {
       private:
        /// Connections for tracking dirty state changes in stores
        Connections _dirtyStoreConnections;

        /// Connections for tracking dirty state changes in settings
        Connections _dirtySettingsConnections;

        /// Connection for tracking when settings are saved, used to update the
        /// window title when settings are saved and there are no dirty stores
        Connection _settingsSavedConnection;

       public:
        void subscribe(
            store::StoreContainer& storeContainer,
            settings::Settings&    settings,
            ui::MainWindow*        mainWindow
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__