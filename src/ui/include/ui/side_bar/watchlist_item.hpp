#ifndef __UI__INCLUDE__UI__SIDE_BAR__WATCHLIST_ITEM_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__WATCHLIST_ITEM_HPP__

#include "config/id_types.hpp"
#include "side_bar_item.hpp"

class QString;   // Forward declaration
class QMenu;     // Forward declaration
class QAction;   // Forward declaration

namespace ui
{
    /**
     * @brief Item representing a single watchlist in the side bar
     *
     */
    class WatchlistItem : public SideBarItem
    {
       private:
        /// The ID of the watchlist this item represents
        WatchlistId _watchlistId;

        /// action to rename the watchlist
        QAction* _renameAction;
        /// action to delete the watchlist
        QAction* _deleteAction;

       public:
        explicit WatchlistItem(WatchlistId id, const QString& name);

        [[nodiscard]] WatchlistId getId() const;

        [[nodiscard]] QAction* getRenameAction() const;
        [[nodiscard]] QAction* getDeleteAction() const;

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__WATCHLIST_ITEM_HPP__
