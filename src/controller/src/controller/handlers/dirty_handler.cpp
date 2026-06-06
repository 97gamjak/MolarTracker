#include "dirty_handler.hpp"

#include "settings/settings.hpp"
#include "store/store_container.hpp"
#include "ui/main_window.hpp"
#include "ui/utils/discard_changes.hpp"

namespace controller
{

    /**
     * @brief Subscribe to dirty state changes in the stores and settings, this
     * will update the window title to indicate that there are unsaved changes
     *
     * @param storeContainer The store container to subscribe to for dirty state
     * changes
     * @param settings The settings to subscribe to for dirty state changes
     * @param mainWindow The main window to update the title for
     */
    void DirtyStateHandler::subscribe(
        store::StoreContainer& storeContainer,
        settings::Settings&    settings,
        ui::MainWindow*        mainWindow
    )
    {
        auto setTitleDirty = [mainWindow](const bool& isDirty)
        {
            // we only want to send a signal in case something is dirty
            // In case we save settings and the store, we handle the dirty state
            // there
            if (isDirty)
            {
                if (mainWindow)
                    mainWindow->setWindowTitle(isDirty);
            }
        };

        _dirtyStoreConnections =
            storeContainer.subscribeToDirty(setTitleDirty, mainWindow);

        _dirtySettingsConnections =
            settings.subscribeToDirty(setTitleDirty, mainWindow);

        _settingsSavedConnection = settings.subscribeToSaved(
            [mainWindow, &storeContainer]()
            {
                if (mainWindow)
                    if (!storeContainer.isDirty())
                        mainWindow->setWindowTitle(false);
            },
            mainWindow
        );

        if (mainWindow)
            mainWindow->setCanCloseCallback(
                [&storeContainer, &settings, mainWindow]()
                {
                    if (!storeContainer.isDirty() && !settings.isDirty())
                        return true;
                    return ui::askDiscardChanges(mainWindow) ==
                           QMessageBox::Yes;
                }
            );
    }

}   // namespace controller