#include "ui/account/account_detail_view.hpp"

#include <qboxlayout.h>
#include <qstackedwidget.h>

#include <QLabel>
#include <QVBoxLayout>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "ui/position/position_table_model.hpp"
#include "ui/position/position_table_view.hpp"
#include "ui/utils/error.hpp"
#include "utils/qt_helpers.hpp"

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

        /// The position table view
        PositionTableView* positionTableView;
        /// The position table model
        PositionTableModel* positionTableModel;

        /// The stacked widget
        QStackedWidget* stackedWidget;

        /// The cash account widget
        QWidget* cashAccountWidget;
        /// The security account widget
        QWidget* securityAccountWidget;

        /// The cash account layout
        QLayout* cashAccountLayout;
        /// The security account layout
        QLayout* securityAccountLayout;

        UIElements();

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
          positionTableView(new PositionTableView()),
          positionTableModel(new PositionTableModel()),
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
        positionTableView->setObjectName("positionTableView");
        positionTableModel->setObjectName("positionTableModel");
        stackedWidget->setObjectName("stackedWidget");
        cashAccountWidget->setObjectName("cashAccountWidget");
        securityAccountWidget->setObjectName("securityAccountWidget");
        cashAccountLayout->setObjectName("cashAccountLayout");
        securityAccountLayout->setObjectName("securityAccountLayout");

        auto* mainLayout = utils::makeQChild<QVBoxLayout>();
        mainLayout->addWidget(titleLabel);
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(balanceLabel);
        mainLayout->addWidget(stackedWidget);

        cashAccountWidget->setLayout(cashAccountLayout);
        stackedWidget->addWidget(cashAccountWidget);

        securityAccountLayout->addWidget(positionTableView);
        positionTableView->setModel(positionTableModel);
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
            "Name: " + QString::fromStdString(_account->name)
        );
        _uiElements->balanceLabel->setText(
            "Balance: " + QString::number(0) + " " +
            QString::fromStdString(CurrencyMeta::toString(_account->currency))
        );
    }

    /**
     * @brief Update the cash account details displayed in the view
     *
     * @param account The account data to display
     */
    void AccountDetailView::updateCashAccount(const AccountDraft& account)
    {
        if (account.kind != AccountKind::Cash)
        {
            ErrorDialog::show(
                std::format(
                    "Invalid account type for account '{}' with id '{}'",
                    account.name,
                    account.id.toString()
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
     * @param positions The positions associated with the account
     */
    void AccountDetailView::updateSecurityAccount(
        const AccountDraft&                             account,
        const std::vector<drafts::PositionDetailDraft>& positions
    )
    {
        if (account.kind != AccountKind::Security)
        {
            ErrorDialog::show(
                std::format(
                    "Invalid account type for account '{}' with id '{}'",
                    account.name,
                    account.id.toString()
                )
            );
            return;
        }

        _updateAccount(account);

        _uiElements->positionTableModel->setPositions(positions);

        _uiElements->stackedWidget->setCurrentWidget(
            _uiElements->securityAccountWidget
        );
    }

}   // namespace ui
