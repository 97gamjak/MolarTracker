#include "side_bar_category_controller.hpp"

#include <QMainWindow>

namespace ui
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
        Category*    category,
        QMainWindow* mainWindow
    )
        : _category(category), _mainWindow(mainWindow)
    {
    }

    /**
     * @brief Get the Category object
     *
     * @return Category*
     */
    Category* SideBarCategoryController::getCategory() const
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

}   // namespace ui