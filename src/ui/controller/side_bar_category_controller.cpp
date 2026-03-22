#include "side_bar_category_controller.hpp"

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
     */
    SideBarCategoryController::SideBarCategoryController(Category* category)
        : _category(category)
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

}   // namespace ui