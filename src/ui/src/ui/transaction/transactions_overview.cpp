#include "ui/transaction/transactions_overview.hpp"

#include <qabstractitemmodel.h>
#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qsortfilterproxymodel.h>
#include <qtableview.h>

#include "drafts/transaction/transaction_overview_draft.hpp"
#include "ui/transaction/cash_transaction_table.hpp"
#include "ui/transaction/stock_transaction_table.hpp"
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
          _cashTitle(new QLabel("Cash Transactions")),
          _stockTitle(new QLabel("Stock Transactions")),
          _cashModel(new CashTransactionTableModel(this)),
          _stockModel(new StockTransactionTableModel(this)),
          _cashProxy(new QSortFilterProxyModel(this)),
          _stockProxy(new QSortFilterProxyModel(this)),
          _cashTable(new QTableView(this)),
          _stockTable(new QTableView(this))
    {
        _cashProxy->setSourceModel(_cashModel);
        _cashProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _cashProxy->setFilterKeyColumn(-1);   // search all columns

        _stockProxy->setSourceModel(_stockModel);
        _stockProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _stockProxy->setFilterKeyColumn(-1);   // search all columns

        _cashTitle->setProperty("class", "sectionTitle");
        _stockTitle->setProperty("class", "sectionTitle");

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

        auto* cashHeader  = _setupTable(_cashTable, _cashProxy);
        auto* stockHeader = _setupTable(_stockTable, _stockProxy);
        _cashTable->sortByColumn(
            CashTransactionTableModel::getDateIndex(),
            Qt::DescendingOrder
        );
        _stockTable->sortByColumn(
            StockTransactionTableModel::getDateIndex(),
            Qt::DescendingOrder
        );
        cashHeader->setSectionResizeMode(
            CashTransactionTableModel::getDescriptionIndex(),
            QHeaderView::Stretch
        );
        stockHeader->setSectionResizeMode(
            StockTransactionTableModel::getDescriptionIndex(),
            QHeaderView::Stretch
        );

        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        layout->addWidget(search);
        layout->addWidget(_cashTitle);
        layout->addWidget(_cashTable);
        layout->addWidget(_stockTitle);
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
        const std::vector<drafts::CashTransactionOverview>&  cashTransactions,
        const std::vector<drafts::StockTransactionOverview>& stockTransactions,
        const IdMap<AccountId, std::string>&                 accountIdToName
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
     *
     * @return QHeaderView* The header view of the configured table, which can
     * be used for further customization if needed.
     */
    QHeaderView* TransactionsOverview::_setupTable(
        QTableView*            table,
        QSortFilterProxyModel* proxy
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

        header->setSortIndicatorShown(true);

        return header;
    }
}   // namespace ui