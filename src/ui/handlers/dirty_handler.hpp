#ifndef __UI__HANDLERS__DIRTY_HANDLER_HPP__
#define __UI__HANDLERS__DIRTY_HANDLER_HPP__

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

namespace ui
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

       public:
        void subscribe(app::AppContext& appContext, ui::MainWindow* mainWindow);
    };

}   // namespace ui

#endif   // __UI__HANDLERS__DIRTY_HANDLER_HPP__