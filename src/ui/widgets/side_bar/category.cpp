#include "category.hpp"

namespace ui
{
    /**
     * @brief Construct a new Category:: Category object
     *
     * @param name The name of the category to display in the side bar
     */
    Category::Category(const QString& name)
        : SideBarItem(name, SideBarItemType::Category)
    {
        setFlags(flags() & ~Qt::ItemIsSelectable);
        QFont categoryFont = font();
        categoryFont.setBold(true);
        setFont(categoryFont);
    }

}   // namespace ui