#include "ui/side_bar/overview_category.hpp"

namespace ui
{
    /**
     * @brief Construct a new Overview Category:: Overview Category object
     *
     */
    OverviewCategory::OverviewCategory()
        : Category("Overview", SideBarItemType::OverviewCategory)
    {
    }

    /**
     * @brief Populate the context menu of the overview category, this will be
     * called when the overview category is right-clicked, and the menu will be
     * shown to the user
     *
     * @param menu The menu to populate, the overview category should add its
     * actions to this menu
     */
    void OverviewCategory::populateContextMenu(QMenu& /*menu*/)
    {
        // No actions for now
        // menu.addAction("Some Action");
    }

}   // namespace ui