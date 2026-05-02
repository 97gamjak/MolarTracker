#include "ui/side_bar/securities_category.hpp"

#include <QMenu>

namespace ui
{
    SecuritiesCategory::SecuritiesCategory()
        : Category("Securities", SideBarItemType::SecuritiesCategory)
    {
    }

    void SecuritiesCategory::populateContextMenu(QMenu& /*menu*/) {}

}   // namespace ui
