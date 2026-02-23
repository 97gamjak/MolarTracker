#include "dirty_handler.hpp"

#include "app/app_context.hpp"
#include "app/store_container.hpp"
#include "settings/settings.hpp"
#include "ui/main_window.hpp"

namespace ui
{

    /**
     * @brief Subscribe to dirty state changes in the stores and settings, this
     * will update the window title to indicate that there are unsaved changes
     *
     * @param storeContainer
     * @param settings
     * @param mainWindow
     */
    void DirtyStateHandler::subscribe(
        app::AppContext& appContext,
        ui::MainWindow*  mainWindow
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

        auto& storeContainer = appContext.getStore();
        auto& settings       = appContext.getSettings();

        _dirtyStoreConnections =
            storeContainer.subscribeToDirty(setTitleDirty, mainWindow);

        _dirtySettingsConnections =
            settings.subscribeToDirty(setTitleDirty, mainWindow);
    }

}   // namespace ui