#include "ui/side_bar/securities_category.hpp"

#include <QMenu>

namespace ui
{
    /**
     * @brief Construct a new Securities Category:: Securities Category object
     *
     */
    SecuritiesCategory::SecuritiesCategory()
        : Category("Securities", SideBarItemType::SecuritiesCategory),
          _createAction(nullptr)
    {
    }

    /**
     * @brief Populate the context menu for the securities category.
     *
     * @param menu The context menu to populate.
     */
    void SecuritiesCategory::populateContextMenu(QMenu& menu)
    {
        _createAction = menu.addAction("Add Security");
    }

    /**
     * @brief Get the "Add Security" action.
     *
     * @return QAction* The "Add Security" action.
     */
    QAction* SecuritiesCategory::getCreateAction() const
    {
        return _createAction;
    }

}   // namespace ui
