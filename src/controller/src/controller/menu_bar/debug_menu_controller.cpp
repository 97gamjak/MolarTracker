#include "debug_menu_controller.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QStatusBar>

#include "commands/undo_stack.hpp"
#include "commands/update_debug_flags_command.hpp"
#include "common/qt_helpers.hpp"
#include "logging/log_macros.hpp"
#include "logging/log_manager.hpp"
#include "settings/settings.hpp"
#include "ui/logging/debug_slots_dialog.hpp"
#include "ui/logging/log_viewer_dialog.hpp"
#include "ui/menu_bar/debug_menu.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.DebugMenuController");

namespace controller
{
    /**
     * @brief Construct a new Debug Menu Controller:: Debug Menu Controller
     * object
     *
     * @param mainWindow
     * @param debugMenu
     * @param undoStack
     * @param settings
     */
    DebugMenuController::DebugMenuController(
        QMainWindow&        mainWindow,
        ui::DebugMenu&      debugMenu,
        cmd::UndoStack&     undoStack,
        settings::Settings& settings
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _debugMenu(debugMenu),
          _undoStack(undoStack),
          _settings(settings)
    {
        connect(
            &debugMenu,
            &ui::DebugMenu::requestDebugSlots,
            this,
            &DebugMenuController::_onRequestDebugSlots
        );

        connect(
            &debugMenu,
            &ui::DebugMenu::requestLogViewer,
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
        const auto& logManager = logging::LogManager::getInstance();
        const auto  categories = logManager.getCategories();

        _debugSlotsDialog->setCategories(categories);
        _debugSlotsDialog->populateTree();

        _debugSlotsDialog->show();
        _debugSlotsDialog->raise();
        _debugSlotsDialog->activateWindow();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar != nullptr)
            statusBar->showMessage("Debug slots opened");
    }

    /**
     * @brief Handle debug slots change request
     *
     * @param action The action to perform (reset, apply, apply and close)
     * @param categories The new debug flag categories to set
     * @param persistChanges Whether to persist changes
     */
    void DebugMenuController::_onDebugSlotsChangeRequested(
        const ui::DebugSlotsDialog::Action& action,
        const logging::LogCategories&       categories,
        bool                                persistChanges
    )
    {
        using enum ui::DebugSlotsDialog::Action;

        switch (action)
        {
            case ResetDefault:
                _resetDefaultDebugFlags();
                break;
            case Apply:
                _applyDebugFlagChanges(categories, persistChanges);
                break;
            case ApplyAndClose:
                _applyDebugFlagChangesAndClose(categories, persistChanges);
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

        if (statusBar != nullptr)
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

        auto debugSlotsSettings =
            std::make_shared<ui::DebugSlotsDialog::Settings>(
                _settings.getUISettings().getDebugSlotsSettings().getWindowSize(
                )
            );

        _debugSlotsDialog = common::makeQChild<ui::DebugSlotsDialog>(
            debugSlotsSettings,
            &_mainWindow
        );

        _debugSlotsDialog->setModal(false);

        connect(
            _debugSlotsDialog,
            &ui::DebugSlotsDialog::requested,
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

        _logViewerDialog = common::makeQChild<ui::LogViewerDialog>(
            _logViewerSettings,
            &_mainWindow
        );

        _logViewerDialog->setModal(false);
    }

    /**
     * @brief Reset debug flags to default values
     *
     */
    void DebugMenuController::_resetDefaultDebugFlags()
    {
        const auto& logManager = logging::LogManager::getInstance();
        const auto  categories = logManager.getDefaultCategories();

        _debugSlotsDialog->setCategories(categories, false);
    }

    /**
     * @brief Discard debug flag changes and reset to current values
     *
     * This is used when there was an error while applying the changes, to
     * reset the dialog to the current values and avoid leaving it in an
     * inconsistent state.
     *
     * @param categories The current debug flag categories to reset to
     * @param persistChanges Whether to persist the changes
     *
     */
    void DebugMenuController::_applyDebugFlagChanges(
        const logging::LogCategories& categories,
        bool                          persistChanges
    )
    {
        auto result = cmd::Commands::makeAndDo<cmd::UpdateDebugFlagsCommand>(
            categories,
            persistChanges
        );

        if (!result)
        {
            // TODO(97gamjak): create general exception message for
            // unexpected errors
            // https://97gamjak.atlassian.net/browse/MOLTRACK-112
            LOG_ERROR(
                "There happened an unexpected error while updating the "
                "debug "
                "flags! Please contact the developer!"
            );

            // this should not happen, but if it does, we should not leave
            // the dialog in an inconsistent state, so we will just reset
            // the categories to the current values and repopulate the tree
            const auto& logManager        = logging::LogManager::getInstance();
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
     * @param persistChanges Whether to persist the changes
     */
    void DebugMenuController::_applyDebugFlagChangesAndClose(
        const logging::LogCategories& categories,
        bool                          persistChanges
    )
    {
        _applyDebugFlagChanges(categories, persistChanges);
        _debugSlotsDialog->accept();
    }

    /**
     * @brief Apply log viewer settings to the dialog
     *
     */
    void DebugMenuController::_applyLogViewerSettings()
    {
        const auto& settings = _settings.getUISettings().getLogViewerSettings();

        _logViewerSettings = std::make_shared<ui::LogViewerDialog::Settings>(
            settings.getReloadIntervalMs(),
            settings.isAutoReloadEnabled(),
            settings.isLineWrapEnabled(),
            settings.getDialogSize(),
            settings.getMaxBlockCount()
        );
    }

}   // namespace controller