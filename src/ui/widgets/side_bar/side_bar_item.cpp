#include "side_bar_item.hpp"

#include <QMenu>
#include <QString>

namespace ui
{
    /**
     * @brief Construct a new Side Bar Item:: Side Bar Item object
     *
     * @param name The name of the item to display in the side bar
     * @param type The type of the item, used to identify which item is selected
     * when the itemSelected signal is emitted
     */
    SideBarItem::SideBarItem(const QString& name, SideBarItemType type)
        : QStandardItem(name), _type(type), _name(name)
    {
        setEditable(false);
    }

    /**
     * @brief Get the type of the item
     *
     * @return SideBarItemType The type of the item, used to identify which item
     * is selected when the itemSelected signal is emitted
     */
    SideBarItemType SideBarItem::getType() const { return _type; }

    /**
     * @brief Set the name of the item, this will update the text displayed in
     * the side bar for this item
     *
     * @param name The new name of the item to display in the side bar
     */
    void SideBarItem::setName(const QString& name)
    {
        _name = name;
        setText(name);
    }

}   // namespace ui