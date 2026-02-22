#ifndef __UI__CONTROLLER__DIRTY_CONTROLLER_HPP__
#define __UI__CONTROLLER__DIRTY_CONTROLLER_HPP__

#include <vector>

#include "connections/connection.hpp"

namespace app
{
    class StoreContainer;   // Forward declaration
}   // namespace app

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace ui
{
    class MainWindow;   // Forward declaration
}   // namespace ui

namespace ui
{
    /**
     * @brief Controller for managing the dirty state of the application, this
     * controller is responsible for tracking the dirty state of the application
     * and providing methods for marking the application as dirty or clean, it
     * can also be used to subscribe to changes in the dirty state and to notify
     * subscribers when the dirty state changes.
     *
     */
    class DirtyStateController
    {
       private:
        /// Connections for tracking dirty state changes in stores
        std::vector<Connection> _dirtyStoreConnections;

        /// Connections for tracking dirty state changes in settings
        std::vector<Connection> _dirtySettingsConnections;

       public:
        explicit DirtyStateController(
            app::StoreContainer& storeContainer,
            settings::Settings&  settings,
            ui::MainWindow*      mainWindow
        );
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__DIRTY_CONTROLLER_HPP__