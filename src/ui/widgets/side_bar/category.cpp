#include "category.hpp"

namespace ui
{
    /**
     * @brief Construct a new Category:: Category object
     *
     * @param name The name of the category to display in the side bar
     * @param type The type of the category, used to identify which item is
     * selected when the itemSelected signal is emitted
     */
    Category::Category(const QString& name, SideBarItemType type)
        : SideBarItem(name, type)
    {
        setFlags(flags() & ~Qt::ItemIsSelectable);
        QFont categoryFont = font();
        categoryFont.setBold(true);
        setFont(categoryFont);
    }

}   // namespace ui