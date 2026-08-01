#ifndef __UI__INCLUDE__UI__SIDE_BAR__ALL_SECURITIES_ITEM_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__ALL_SECURITIES_ITEM_HPP__

#include "side_bar_item.hpp"

class QMenu;   // Forward declaration

namespace ui
{
    /**
     * @brief Permanent child item of SecuritiesCategory representing the
     * unfiltered "All Securities" view. Always present, selected by default.
     *
     */
    class AllSecuritiesItem : public SideBarItem
    {
       public:
        explicit AllSecuritiesItem();

        void populateContextMenu(QMenu& menu) override;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__ALL_SECURITIES_ITEM_HPP__
