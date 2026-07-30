#include "ui/account/account_detail_view.hpp"

#include <qboxlayout.h>
#include <qstackedwidget.h>

#include <QLabel>
#include <QVBoxLayout>

#include "common/finance.hpp"
#include "common/qt_helpers.hpp"
#include "drafts/account_draft.hpp"
#include "ui/position/option_position_table_model.hpp"
#include "ui/position/option_position_table_view.hpp"
#include "ui/position/stock_position_table_model.hpp"
#include "ui/position/stock_position_table_view.hpp"
#include "ui/utils/error.hpp"

namespace ui
{
    using drafts::AccountDraft;

    /**
     * @brief UI elements for the account detail view
     *
     */
    struct AccountDetailView::UIElements
    {
        /// The title label
        QLabel* titleLabel;
        /// The name label
        QLabel* nameLabel;
        /// The balance label
        QLabel* balanceLabel;

        /// The stock position table view
        StockPositionTableView* stockTableView;
        /// The option position table view
        OptionPositionTableView* optionTableView;
        /// The stock position table model
        StockPositionTableModel* stockTable;
        /// The option position table model
        OptionPositionTableModel* optionTable;
        /// The stock positions section title
        QLabel* stockTableTitle;
        /// The option positions section title
        QLabel* optionTableTitle;

        /// The stacked widget
        QStackedWidget* stackedWidget;

        /// The cash account widget
        // cppcheck-suppress unsafeClassCanLeak -- Qt owned
        QWidget* cashAccountWidget;
        /// The security account widget
        // cppcheck-suppress unsafeClassCanLeak -- Qt owned
        QWidget* securityAccountWidget;

        /// The cash account layout
        QLayout* cashAccountLayout;
        /// The security account layout
        QLayout* securityAccountLayout;

        UIElements();
        ~UIElements()                            = default;
        UIElements(const UIElements&)            = delete;
        UIElements(UIElements&&)                 = delete;
        UIElements& operator=(const UIElements&) = delete;
        UIElements& operator=(UIElements&&)      = delete;

        [[nodiscard]] QLayout* setupUI() const;
    };

    /**
     * @brief Construct a new UIElements object
     *
     */
    AccountDetailView::UIElements::UIElements()
        : titleLabel(new QLabel("Account Details")),
          nameLabel(new QLabel()),
          balanceLabel(new QLabel()),
          stockTableView(new StockPositionTableView()),
          optionTableView(new OptionPositionTableView()),
          stockTable(new StockPositionTableModel()),
          optionTable(new OptionPositionTableModel()),
          stockTableTitle(new QLabel("Stock Positions")),
          optionTableTitle(new QLabel("Option Positions")),
          stackedWidget(new QStackedWidget()),
          cashAccountWidget(new QWidget()),
          securityAccountWidget(new QWidget()),
          cashAccountLayout(new QVBoxLayout()),
          securityAccountLayout(new QVBoxLayout())
    {
    }

    /**
     * @brief Setup the UI elements
     *
     * @return The layout containing the UI elements
     */
    QLayout* AccountDetailView::UIElements::setupUI() const
    {
        // Setup the UI elements
        titleLabel->setObjectName("titleLabel");
        nameLabel->setObjectName("nameLabel");
        balanceLabel->setObjectName("balanceLabel");
        stockTableView->setObjectName("stockTableView");
        stockTable->setObjectName("stockTable");
        optionTableView->setObjectName("optionTableView");
        optionTable->setObjectName("optionTable");
        stockTableTitle->setObjectName("stockTableTitle");
        optionTableTitle->setObjectName("optionTableTitle");
        stackedWidget->setObjectName("stackedWidget");
        cashAccountWidget->setObjectName("cashAccountWidget");
        securityAccountWidget->setObjectName("securityAccountWidget");
        cashAccountLayout->setObjectName("cashAccountLayout");
        securityAccountLayout->setObjectName("securityAccountLayout");

        auto* mainLayout = common::makeQChild<QVBoxLayout>();
        mainLayout->addWidget(titleLabel);
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(balanceLabel);
        mainLayout->addWidget(stackedWidget);

        cashAccountWidget->setLayout(cashAccountLayout);
        stackedWidget->addWidget(cashAccountWidget);

        stockTableTitle->setProperty("class", "sectionTitle");
        optionTableTitle->setProperty("class", "sectionTitle");

        securityAccountLayout->addWidget(stockTableTitle);
        securityAccountLayout->addWidget(stockTableView);
        securityAccountLayout->addWidget(optionTableTitle);
        securityAccountLayout->addWidget(optionTableView);
        stockTableView->setModel(stockTable);
        optionTableView->setModel(optionTable);
        securityAccountWidget->setLayout(securityAccountLayout);
        stackedWidget->addWidget(securityAccountWidget);

        return mainLayout;
    }

    /**
     * @brief Construct a new Account Detail View object
     *
     * @param parent The parent widget
     */
    AccountDetailView::AccountDetailView(QWidget* parent)
        : QWidget(parent), _uiElements(std::make_unique<UIElements>())
    {
        setLayout(_uiElements->setupUI());
    }

    /**
     * @brief Destroy the Account Detail View object
     *
     */
    AccountDetailView::~AccountDetailView() = default;

    /**
     * @brief Update the account details displayed in the view
     *
     * @param account The account data to display
     */
    void AccountDetailView::_updateAccount(const AccountDraft& account)
    {
        _account = std::make_unique<AccountDraft>(account);
        // Update the UI with account details

        // Update the UI elements with the new account details
        _uiElements->nameLabel->setText(
            "Name: " + QString::fromStdString(_account->getName())
        );
        _uiElements->balanceLabel->setText(
            "Balance: " + QString::number(0) + " " +
            QString::fromStdString(
                CurrencyMeta::toString(_account->getCurrency())
            )
        );
    }

    /**
     * @brief Update the cash account details displayed in the view
     *
     * @param account The account data to display
     */
    void AccountDetailView::updateCashAccount(const AccountDraft& account)
    {
        if (account.getKind() != AccountKind::Cash)
        {
            ErrorDialog::show(
                std::format(
                    "Invalid account type for account '{}' with id '{}'",
                    account.getName(),
                    account.getId().toString()
                )
            );
            return;
        }

        _updateAccount(account);

        _uiElements->stackedWidget->setCurrentWidget(
            _uiElements->cashAccountWidget
        );
    }

    /**
     * @brief Update the security account details displayed in the view
     *
     * @param account The account data to display
     * @param stocks The stock positions associated with the account
     * @param options The option positions associated with the account
     */
    void AccountDetailView::updateSecurityAccount(
        const AccountDraft&                                   account,
        const std::vector<drafts::PositionStockDetailDraft>&  stocks,
        const std::vector<drafts::PositionOptionDetailDraft>& options
    )
    {
        if (account.getKind() != AccountKind::Security)
        {
            ErrorDialog::show(
                std::format(
                    "Invalid account type for account '{}' with id '{}'",
                    account.getName(),
                    account.getId().toString()
                )
            );
            return;
        }

        _updateAccount(account);

        _uiElements->stockTable->setPositions(stocks);
        _uiElements->optionTable->setPositions(options);

        _uiElements->stackedWidget->setCurrentWidget(
            _uiElements->securityAccountWidget
        );
    }

}   // namespace ui
