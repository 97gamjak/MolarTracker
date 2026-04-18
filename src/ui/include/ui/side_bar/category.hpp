#ifndef __UI__INCLUDE__UI__SIDE_BAR__CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__CATEGORY_HPP__

#include "side_bar_item.hpp"

namespace ui
{
    /**
     * @brief Category in the side bar, this is a special type of item that can
     * contain other items, and has a context menu for actions related to the
     * category as a whole (e.g., creating a new account in the accounts
     * category)
     *
     */
    class Category : public SideBarItem
    {
       public:
        explicit Category(const QString& name, SideBarItemType type);

        /**
         * @brief Populate the context menu of the category, this will be called
         * when the category is right-clicked, and the menu will be shown to the
         * user
         *
         * @param menu The menu to populate, the category should add its actions
         * to this menu
         */
        void populateContextMenu(QMenu& menu) override = 0;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__CATEGORY_HPP__