#ifndef __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__

#include "category.hpp"
#include "config/id_types.hpp"
#include "ui/side_bar/all_securities_item.hpp"

class QAction;   // Forward declaration
class QString;   // Forward declaration

namespace ui
{
    /**
     * @brief Category for managing securities-related UI components.
     *
     * This class provides the necessary methods to handle the securities
     * category within the side bar.
     */
    class SecuritiesCategory : public Category
    {
       private:
        /// Pointer to the "Add Security" action.
        QAction* _createAction;
        /// Pointer to the "Create Watchlist" action.
        QAction* _createWatchlistAction;

        /// Permanent, always-present "All Securities" child item (row 0).
        AllSecuritiesItem* _allSecuritiesItem;

       public:
        SecuritiesCategory();

        void addWatchlist(WatchlistId id, const QString& name);
        void clearWatchlists();

        void populateContextMenu(QMenu& menu) override;

        [[nodiscard]] QAction*           getCreateAction() const;
        [[nodiscard]] QAction*           getCreateWatchlistAction() const;
        [[nodiscard]] AllSecuritiesItem* getAllSecuritiesItem() const;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
