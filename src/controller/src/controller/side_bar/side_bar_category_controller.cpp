#include "side_bar_category_controller.hpp"

#include <QMainWindow>

#include "logging/log_macros.hpp"
#include "ui/side_bar/side_bar_item.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.SideBarCategoryController");

namespace controller
{
    /**
     * @brief Construct a new Side Bar Category Controller:: Side Bar Category
     * Controller object
     *
     * @param category The category that this controller will manage, the
     * controller will be responsible for refreshing the category when the
     * underlying data changes, and providing an interface for managing the
     * state of the category and its items
     * @param mainWindow A pointer to the main window, this is used as the
     * parent widget for dialogs that are opened as a result of actions in the
     * category (e.g. the create account dialog), this ensures that the dialogs
     * are properly parented in the UI and will be modal to the main window when
     * opened.
     */
    SideBarCategoryController::SideBarCategoryController(
        ui::Category* category,
        QMainWindow*  mainWindow
    )
        : _category(category), _mainWindow(mainWindow)
    {
    }

    /**
     * @brief Get the Category object
     *
     * @return ui::Category*
     */
    ui::Category* SideBarCategoryController::getCategory() const
    {
        return _category;
    }

    /**
     * @brief Get the Main Window object, this is used as the parent widget for
     * dialogs that are opened as a result of actions in the category (e.g. the
     * create account dialog), this ensures that the dialogs are properly
     * parented in the UI and will be modal to the main window when opened.
     *
     * @return QMainWindow*
     */
    QMainWindow* SideBarCategoryController::getMainWindow()
    {
        return _mainWindow;
    }

    /**
     * @brief Handle a triggered action for an item in the category, this will
     * determine which item the action was triggered for and which action was
     * triggered, and perform the appropriate action based on that information
     *
     * @param item The item for which the action was triggered, this should be a
     * pointer to a SideBarItem that is currently in the category, and will be
     * used to determine which item the action was triggered for
     * @param action The action that was triggered, this should be a pointer to
     * a QAction that is currently in the context menu for the item, and will be
     * used to determine which action was triggered
     */
    void SideBarCategoryController::handleTriggeredAction(
        ui::SideBarItem* item,
        SideBarAction    action,
        const QAction*   qaction
    )
    {
        switch (action)
        {
            case SideBarAction::ContextMenuRequested:
                _onContextMenuRequested(item, qaction);
                break;
            case SideBarAction::ItemClicked:
                _onItemClicked(item);
                break;
            case SideBarAction::ItemDoubleClicked:
                _onItemDoubleClicked(item);
                break;
        }
    }

    /**
     * @brief Handle an item being clicked in the category, this will determine
     * which item was clicked and perform the appropriate action, such as
     * opening the corresponding page in the central stack
     *
     * @param item The item that was clicked, this should be a pointer to a
     * SideBarItem that is currently in the category, and will be used to
     * determine which item was clicked and what action to perform
     */
    void SideBarCategoryController::_onItemClicked(ui::SideBarItem* item)
    {
        if (item == nullptr)
        {
            LOG_ERROR(
                "SideBarCategoryController::_onItemClicked called with null "
                "item"
            );
            return;
        }

        const auto type = item->getType();

        LOG_ERROR(
            "SideBarCategoryController::_onItemClicked not implemented for "
            "type: " +
            ui::SideBarItemTypeMeta::toString(type)
        );
    }

    /**
     * @brief Handle a context menu action being triggered for an item in the
     * category, this will determine which item the action was triggered for and
     * which action was triggered, and perform the appropriate action based on
     * that information
     *
     * @param item The item for which the context menu action was triggered,
     * this should be a pointer to a SideBarItem that is currently in the
     * category, and will be used to determine which item the action was
     * triggered for
     * @param action The action that was triggered, this should be a pointer to
     * a QAction that is currently in the context menu for the item, and will be
     * used to determine which action was triggered
     */
    void SideBarCategoryController::_onContextMenuRequested(
        ui::SideBarItem* item,
        const QAction*   action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_ERROR(
                "SideBarCategoryController::_onContextMenuRequested called "
                "with null item or action"
            );
            return;
        }

        const auto type = item->getType();

        LOG_ERROR(
            "SideBarCategoryController::_onContextMenuRequested not "
            "implemented for type: " +
            ui::SideBarItemTypeMeta::toString(type)
        );
    }

    /**
     * @brief Handle an item being double clicked in the category, this will
     * determine which item was double clicked and perform the appropriate
     * action, such as opening the corresponding page in the central stack
     *
     * @param item The item that was double clicked, this should be a pointer to
     * a SideBarItem that is currently in the category, and will be used to
     * determine which item was double clicked and what action to perform
     */
    void SideBarCategoryController::_onItemDoubleClicked(ui::SideBarItem* item)
    {
        if (item == nullptr)
        {
            LOG_ERROR(
                "SideBarCategoryController::_onItemDoubleClicked called with "
                "null item"
            );
            return;
        }

        const auto type = item->getType();

        LOG_ERROR(
            "SideBarCategoryController::_onItemDoubleClicked not implemented "
            "for type: " +
            ui::SideBarItemTypeMeta::toString(type)
        );
    }
}   // namespace controller
