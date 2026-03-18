#ifndef __UI__WIDGETS__SIDE_BAR__CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__CATEGORY_HPP__

#include "side_bar_item.hpp"

namespace ui
{
    class Category : public SideBarItem
    {
       public:
        explicit Category(const QString& name);

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

#endif   // __UI__WIDGETS__SIDE_BAR__CATEGORY_HPP__