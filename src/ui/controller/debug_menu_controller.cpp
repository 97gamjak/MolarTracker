#include "debug_menu_controller.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QStatusBar>

#include "app/app_context.hpp"
#include "logging/log_manager.hpp"
#include "ui/commands/undo_stack.hpp"
#include "ui/commands/update_debug_flags_command.hpp"
#include "ui/widgets/logging/debug_slots_dialog.hpp"
#include "ui/widgets/logging/log_viewer_dialog.hpp"
#include "ui/widgets/menu_bar/debug_menu.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_debugMenuController
#include "logging/log_macros.hpp"

namespace ui
{
    /**
     * @brief Construct a new Debug Menu Controller:: Debug Menu Controller
     * object
     *
     * @param mainWindow
     * @param debugMenu
     * @param appContext
     * @param undoStack
     */
    DebugMenuController::DebugMenuController(
        QMainWindow&     mainWindow,
        DebugMenu&       debugMenu,
        app::AppContext& appContext,
        UndoStack&       undoStack
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _debugMenu(debugMenu),
          _appContext(appContext),
          _undoStack(undoStack)
    {
        connect(
            &debugMenu,
            &DebugMenu::requestDebugSlots,
            this,
            &DebugMenuController::_onRequestDebugSlots
        );

        connect(
            &debugMenu,
            &DebugMenu::requestLogViewer,
            this,
            &DebugMenuController::_onRequestLogViewer
        );
    }

    /**
     * @brief Handle log viewer request
     *
     */
    void DebugMenuController::_onRequestDebugSlots()
    {
        _ensureDebugSlotsDialog();

        // we need to update here the logging categories as
        // an undo can always happen outside of the dialog
        // and hence it could be out of sync
        const auto& logManager = LogManager::getInstance();
        const auto  categories = logManager.getCategories();

        _debugSlotsDialog->setCategories(categories);
        _debugSlotsDialog->populateTree();

        _debugSlotsDialog->show();
        _debugSlotsDialog->raise();
        _debugSlotsDialog->activateWindow();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Debug slots opened");
    }

    /**
     * @brief Handle debug slots change request
     *
     * @param action The action to perform (reset, apply, apply and close)
     * @param categories The new debug flag categories to set
     */
    void DebugMenuController::_onDebugSlotsChangeRequested(
        const DebugSlotsDialog::Action& action,
        const LogCategoryMap&           categories
    )
    {
        using enum DebugSlotsDialog::Action;

        switch (action)
        {
            case ResetDefault:
                _resetDefaultDebugFlags();
                break;
            case Apply:
                _applyDebugFlagChanges(categories);
                break;
            case ApplyAndClose:
                _applyDebugFlagChangesAndClose(categories);
                break;
        }
    }

    /**
     * @brief Handle debug slots request
     *
     */
    void DebugMenuController::_onRequestLogViewer()
    {
        _ensureLogViewerDialog();

        _logViewerDialog->show();
        _logViewerDialog->raise();
        _logViewerDialog->activateWindow();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Log File opened");
    }

    /**
     * @brief Ensures that the Debug Slots dialog is created
     *
     */
    void DebugMenuController::_ensureDebugSlotsDialog()
    {
        if (_debugSlotsDialog != nullptr)
            return;

        _debugSlotsDialog = new DebugSlotsDialog{&_mainWindow};
        _debugSlotsDialog->setModal(false);

        connect(
            _debugSlotsDialog,
            &DebugSlotsDialog::requested,
            this,
            &DebugMenuController::_onDebugSlotsChangeRequested
        );
    }

    /**
     * @brief Ensures that the Log Viewer dialog is created
     *
     */
    void DebugMenuController::_ensureLogViewerDialog()
    {
        if (_logViewerDialog != nullptr)
            return;

        _applyLogViewerSettings();

        _logViewerDialog =
            new LogViewerDialog{_logViewerSettings, &_mainWindow};

        _logViewerDialog->setModal(false);
    }

    /**
     * @brief Reset debug flags to default values
     *
     */
    void DebugMenuController::_resetDefaultDebugFlags()
    {
        const auto& logManager = LogManager::getInstance();
        const auto  categories = logManager.getDefaultCategories();

        _debugSlotsDialog->setCategories(categories, false);
        _debugSlotsDialog->populateTree();
    }

    /**
     * @brief Discard debug flag changes and reset to current values
     *
     * This is used when there was an error while applying the changes, to reset
     * the dialog to the current values and avoid leaving it in an inconsistent
     * state.
     *
     * @param categories The current debug flag categories to reset to
     *
     */
    void DebugMenuController::_applyDebugFlagChanges(
        const LogCategoryMap& categories
    )
    {
        auto result = Commands::makeAndDo<UpdateDebugFlagsCommand>(categories);

        if (!result)
        {
            // TODO: create general exception message for unexpected errors
            // https://97gamjak.atlassian.net/browse/MOLTRACK-112
            LOG_ERROR(
                "There happened an unexpected error while updating the debug "
                "flags! Please contact the developer!"
            );

            // this should not happen, but if it does, we should not leave the
            // dialog in an inconsistent state, so we will just reset the
            // categories to the current values and repopulate the tree
            const auto& logManager        = LogManager::getInstance();
            const auto  currentCategories = logManager.getCategories();
            _debugSlotsDialog->setCategories(currentCategories, false);
            _debugSlotsDialog->populateTree();

            return;
        }

        _undoStack.push(std::move(result).value());

        _debugSlotsDialog->setCategories(categories);
    }

    /**
     * @brief Apply debug flag changes and close the dialog
     *
     * @param categories The new debug flag categories to set
     */
    void DebugMenuController::_applyDebugFlagChangesAndClose(
        const LogCategoryMap& categories
    )
    {
        _applyDebugFlagChanges(categories);
        _debugSlotsDialog->accept();
    }

    /**
     * @brief Apply log viewer settings to the dialog
     *
     */
    void DebugMenuController::_applyLogViewerSettings()
    {
        const auto& settings =
            _appContext.getSettings().getUISettings().getLogViewerSettings();

        _logViewerSettings.setAutoReload(settings.isAutoReloadEnabled());
        _logViewerSettings.setIntervalSec(settings.getReloadIntervalSec());
        _logViewerSettings.setLineWrap(settings.isLineWrapEnabled());
    }

}   // namespace ui