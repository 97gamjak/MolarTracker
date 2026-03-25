#ifndef __UI__INCLUDE__UI__SIDE_BAR__OVERVIEW_CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__OVERVIEW_CATEGORY_HPP__

#include "category.hpp"

namespace ui
{
    /**
     * @brief Category for the overview section in the side bar
     *
     */
    class OverviewCategory : public Category
    {
       public:
        explicit OverviewCategory();

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__OVERVIEW_CATEGORY_HPP__