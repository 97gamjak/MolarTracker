#include "ui/side_bar/securities_category.hpp"

#include <QMenu>
#include <QString>

#include "common/qt_helpers.hpp"
#include "ui/side_bar/all_securities_item.hpp"
#include "ui/side_bar/watchlist_item.hpp"

namespace ui
{
    /**
     * @brief Construct a new Securities Category:: Securities Category object
     *
     */
    SecuritiesCategory::SecuritiesCategory()
        : Category("Securities", SideBarItemType::SecuritiesCategory),
          _createAction(nullptr),
          _createWatchlistAction(nullptr),
          _allSecuritiesItem(new AllSecuritiesItem())
    {
        // permanent row 0, always present, never rebuilt by clearWatchlists()
        appendRow(_allSecuritiesItem);
    }

    /**
     * @brief Add a watchlist to the category, this will create a new
     * watchlist item and add it to the category in the side bar
     *
     * @param id The id of the watchlist
     * @param name The name of the watchlist to display in the side bar
     */
    void SecuritiesCategory::addWatchlist(WatchlistId id, const QString& name)
    {
        auto* watchlistItem = common::makeQChild<WatchlistItem>(id, name);
        appendRow(watchlistItem);
    }

    /**
     * @brief Clear all watchlists from the category. Row 0 is the permanent
     * "All Securities" item and must never be removed, unlike
     * AccountCategory::clearAccounts(), which has no such permanent row.
     *
     */
    void SecuritiesCategory::clearWatchlists()
    {
        if (rowCount() > 1)
            removeRows(1, rowCount() - 1);
    }

    /**
     * @brief Populate the context menu for the securities category.
     *
     * @param menu The context menu to populate.
     */
    void SecuritiesCategory::populateContextMenu(QMenu& menu)
    {
        _createAction          = menu.addAction("Add Security");
        _createWatchlistAction = menu.addAction("Create Watchlist");
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

    /**
     * @brief Get the "Create Watchlist" action.
     *
     * @return QAction* The "Create Watchlist" action.
     */
    QAction* SecuritiesCategory::getCreateWatchlistAction() const
    {
        return _createWatchlistAction;
    }

    /**
     * @brief Get the permanent "All Securities" child item.
     *
     * @return AllSecuritiesItem*
     */
    AllSecuritiesItem* SecuritiesCategory::getAllSecuritiesItem() const
    {
        return _allSecuritiesItem;
    }

}   // namespace ui
