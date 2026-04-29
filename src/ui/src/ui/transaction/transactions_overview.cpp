#include "ui/transaction/transactions_overview.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>

#include "ui/transaction/transaction_table.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Transactions Overview:: Transactions Overview
     * object
     *
     * @param parent
     */
    TransactionsOverview::TransactionsOverview(QWidget* parent)
        : QWidget(parent),
          _model(utils::makeQChild<TransactionTableModel>(this)),
          _proxy(utils::makeQChild<QSortFilterProxyModel>(this)),
          _table(utils::makeQChild<QTableView>(this))
    {
        _proxy->setSourceModel(_model);
        _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _proxy->setFilterKeyColumn(-1);   // search all columns

        auto* search = utils::makeQChild<QLineEdit>(this);
        search->setPlaceholderText("Search transactions…");

        connect(
            search,
            &QLineEdit::textChanged,
            _proxy,
            &QSortFilterProxyModel::setFilterFixedString
        );

        _setupTable();

        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        layout->addWidget(search);
        layout->addWidget(_table);
    }

    /**
     * @brief Refresh the transactions displayed in the overview
     *
     * @param transactions
     * @param accountIdToName
     */
    void TransactionsOverview::refresh(
        const std::vector<drafts::TransactionOverviewDraft>& transactions,
        const std::unordered_map<AccountId, std::string, AccountId::Hash>&
            accountIdToName
    )
    {
        _model->setTransactions(transactions, accountIdToName);
    }

    /**
     * @brief Set up the transaction table view, this will configure the table
     * view to display the transactions in a user-friendly way, including
     * setting up sorting, selection behavior, and column resizing to ensure
     * that the transaction data is presented clearly and is easy to navigate
     * for the user.
     *
     */
    void TransactionsOverview::_setupTable()
    {
        _table->setModel(_proxy);
        _table->setSortingEnabled(true);
        _table->setSelectionBehavior(QAbstractItemView::SelectRows);
        _table->setSelectionMode(QAbstractItemView::SingleSelection);
        _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _table->setAlternatingRowColors(true);
        _table->verticalHeader()->hide();

        auto* header = _table->horizontalHeader();
        header->setStretchLastSection(false);
        header->setSectionResizeMode(

            QHeaderView::ResizeToContents
        );

        // resize to content size
        header->setSectionResizeMode(QHeaderView::ResizeToContents);

        // description takes remaining space
        header->setSectionResizeMode(
            TransactionTableModel::getDescriptionIndex(),
            QHeaderView::Stretch
        );

        header->setSortIndicatorShown(true);

        // sensible default sort: newest first
        _table->sortByColumn(
            TransactionTableModel::getDateIndex(),
            Qt::DescendingOrder
        );
    }
}   // namespace ui