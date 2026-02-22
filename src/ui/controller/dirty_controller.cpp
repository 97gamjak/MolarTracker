#include "dirty_controller.hpp"

#include "app/store_container.hpp"
#include "logging/log_macros.hpp"
#include "settings/settings.hpp"
#include "ui/main_window.hpp"

namespace ui
{

    /**
     * @brief Construct a new Dirty Controller:: Dirty Controller object
     *
     * @param storeContainer
     * @param settings
     * @param mainWindow
     */
    DirtyStateController::DirtyStateController(
        app::StoreContainer& storeContainer,
        settings::Settings&  settings,
        ui::MainWindow*      mainWindow
    )
    {
        auto setTitleDirty = [](void* user, const bool& isDirty)
        {
            // we only want to send a signal in case something is dirty
            // In case we save settings and the store, we handle the dirty state
            // there
            if (isDirty)
            {
                if (auto* mainWindow = static_cast<ui::MainWindow*>(user))
                    mainWindow->setWindowTitle(isDirty);
            }
        };

        _dirtyStoreConnections =
            storeContainer.subscribeToDirty(setTitleDirty, mainWindow);

        _dirtySettingsConnections =
            settings.subscribeToDirty(setTitleDirty, mainWindow);
    }

}   // namespace ui