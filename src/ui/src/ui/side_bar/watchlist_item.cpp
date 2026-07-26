#include "ui/side_bar/watchlist_item.hpp"

#include <QAction>
#include <QMenu>
#include <QString>

namespace ui
{
    /**
     * @brief Construct a new Watchlist Item:: Watchlist Item object
     *
     * @param id The id of the watchlist
     * @param name The name of the watchlist to display in the side bar
     */
    WatchlistItem::WatchlistItem(WatchlistId id, const QString& name)
        : SideBarItem(name, SideBarItemType::WatchlistItem),
          _watchlistId(id),
          _renameAction(nullptr),
          _deleteAction(nullptr)
    {
    }

    /**
     * @brief Get the id of the watchlist
     *
     * @return WatchlistId
     */
    WatchlistId WatchlistItem::getId() const { return _watchlistId; }

    /**
     * @brief Get the rename action of the watchlist item
     *
     * @return QAction*
     */
    QAction* WatchlistItem::getRenameAction() const { return _renameAction; }

    /**
     * @brief Get the delete action of the watchlist item
     *
     * @return QAction*
     */
    QAction* WatchlistItem::getDeleteAction() const { return _deleteAction; }

    /**
     * @brief Populate the context menu of the watchlist item
     *
     * @param menu The menu to populate
     */
    void WatchlistItem::populateContextMenu(QMenu& menu)
    {
        _renameAction = menu.addAction("Rename");
        _deleteAction = menu.addAction("Delete");
    }

}   // namespace ui
