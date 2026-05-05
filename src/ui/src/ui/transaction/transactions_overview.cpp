#include "ui/transaction/transactions_overview.hpp"

#include <qabstractitemmodel.h>
#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qsortfilterproxymodel.h>
#include <qtableview.h>

#include "ui/transaction/cash_transaction_table.hpp"
#include "ui/transaction/stock_transaction_table.hpp"
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
          _cashModel(utils::makeQChild<CashTransactionTableModel>(this)),
          _stockModel(utils::makeQChild<StockTransactionTableModel>(this)),
          _cashProxy(utils::makeQChild<QSortFilterProxyModel>(this)),
          _stockProxy(utils::makeQChild<QSortFilterProxyModel>(this)),
          _cashTable(utils::makeQChild<QTableView>(this)),
          _stockTable(utils::makeQChild<QTableView>(this))
    {
        _cashProxy->setSourceModel(_cashModel);
        _cashProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _cashProxy->setFilterKeyColumn(-1);   // search all columns

        _stockProxy->setSourceModel(_stockModel);
        _stockProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _stockProxy->setFilterKeyColumn(-1);   // search all columns

        auto* search = utils::makeQChild<QLineEdit>(this);
        search->setPlaceholderText("Search transactions…");

        connect(
            search,
            &QLineEdit::textChanged,
            _cashProxy,
            &QSortFilterProxyModel::setFilterFixedString
        );

        connect(
            search,
            &QLineEdit::textChanged,
            _stockProxy,
            &QSortFilterProxyModel::setFilterFixedString
        );

        _setupTable(_cashTable, _cashProxy, _cashModel);
        _setupTable(_stockTable, _stockProxy, _stockModel);

        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        layout->addWidget(search);
        layout->addWidget(_cashTable);
        layout->addWidget(_stockTable);
    }

    /**
     * @brief Refresh the transactions displayed in the overview
     *
     * @param cashTransactions The list of cash transactions to display.
     * @param stockTransactions The list of stock transactions to display.
     * @param accountIdToName A mapping of account IDs to account names for
     * display purposes.
     */
    void TransactionsOverview::refresh(
        const std::vector<drafts::TransactionOverviewDraft>& cashTransactions,
        const std::vector<drafts::TransactionOverviewDraft>& stockTransactions,
        const std::unordered_map<AccountId, std::string, AccountId::Hash>&
            accountIdToName
    )
    {
        _cashModel->setTransactions(cashTransactions, accountIdToName);
        _stockModel->setTransactions(stockTransactions, accountIdToName);
    }

    /**
     * @brief Set up the transaction table view, this will configure the table
     * view to display the transactions in a user-friendly way, including
     * setting up sorting, selection behavior, and column resizing to ensure
     * that the transaction data is presented clearly and is easy to navigate
     * for the user.
     *
     * @param table The table view to set up.
     * @param proxy The proxy model to use for filtering.
     * @param model The model to use for the table view.
     *
     */
    void TransactionsOverview::_setupTable(
        QTableView*            table,
        QSortFilterProxyModel* proxy,
        TransactionTableModel* model
    )
    {
        table->setModel(proxy);
        table->setSortingEnabled(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setAlternatingRowColors(true);
        table->verticalHeader()->hide();

        auto* header = table->horizontalHeader();
        header->setStretchLastSection(false);
        header->setSectionResizeMode(

            QHeaderView::ResizeToContents
        );

        // resize to content size
        header->setSectionResizeMode(QHeaderView::ResizeToContents);

        // description takes remaining space
        header->setSectionResizeMode(
            model->getDescriptionIndex(),
            QHeaderView::Stretch
        );

        header->setSortIndicatorShown(true);

        // sensible default sort: newest first
        table->sortByColumn(model->getDateIndex(), Qt::DescendingOrder);
    }
}   // namespace ui