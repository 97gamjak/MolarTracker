#ifndef __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__

#include <vector>

#include "connections/connection.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace ui
{
    class MainWindow;   // Forward declaration
}   // namespace ui

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
        std::vector<Connection> _dirtyStoreConnections;

        /// Connections for tracking dirty state changes in settings
        std::vector<Connection> _dirtySettingsConnections;

        /// Connection for tracking when settings are saved, used to update the
        /// window title when settings are saved and there are no dirty stores
        Connection _settingsSavedConnection;

       public:
        void subscribe(app::AppContext& appContext, ui::MainWindow* mainWindow);
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HANDLERS__DIRTY_HANDLER_HPP__