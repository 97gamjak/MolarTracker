#ifndef __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_ITEM_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_ITEM_HPP__

#include <QStandardItem>
#include <cstdint>
#include <mstd/enum.hpp>

class QString;   // Forward declaration
class QMenu;     // Forward declaration

namespace ui
{
    class SideBar;

    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define SIDE_BAR_ITEM_TYPE_LIST(X) \
    X(AccountCategory)             \
    X(OverviewCategory)            \
    X(AccountsItem)

    MSTD_ENUM(SideBarItemType, std::uint8_t, SIDE_BAR_ITEM_TYPE_LIST);

    /**
     * @brief Base class for items in the side bar, this is used to identify
     * which item is selected when the itemSelected signal is emitted, and to
     * populate the context menu of the item when it is right-clicked
     *
     */
    class SideBarItem : public QStandardItem
    {
       private:
        /// The type of the item
        SideBarItemType _type;

        /// The name of the item
        QString _name;

       public:
        explicit SideBarItem(const QString& name, SideBarItemType type);

        [[nodiscard]] SideBarItemType getType() const;

        void setName(const QString& name);

        /**
         * @brief Populate the context menu of the item, this will be called
         * when the item is right-clicked, and the menu will be shown to the
         * user
         *
         * @param menu The menu to populate, the item should add its actions to
         * this menu
         */
        virtual void populateContextMenu(QMenu& menu) = 0;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_ITEM_HPP__