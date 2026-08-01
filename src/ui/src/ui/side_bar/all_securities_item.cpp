#include "ui/side_bar/all_securities_item.hpp"

namespace ui
{
    /**
     * @brief Construct a new All Securities Item:: All Securities Item object
     *
     */
    AllSecuritiesItem::AllSecuritiesItem()
        : SideBarItem("All Securities", SideBarItemType::AllSecuritiesItem)
    {
    }

    /**
     * @brief Populate the context menu of the "All Securities" item. This is
     * a permanent, immutable node, so no actions are added — the side bar
     * skips showing a menu when there are none.
     *
     * @param menu The menu to populate
     */
    void AllSecuritiesItem::populateContextMenu(QMenu& /*menu*/) {}

}   // namespace ui
