#include "ui/account/account_detail_view.hpp"

#include <QLabel>
#include <QVBoxLayout>

#include "config/finance_enums.hpp"
#include "drafts/account_draft.hpp"
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
    };

    /**
     * @brief Construct a new Account Detail View object
     *
     * @param parent The parent widget
     */
    AccountDetailView::AccountDetailView(QWidget* parent) : QWidget(parent)
    {
        _setupUi();
    }

    /**
     * @brief Destroy the Account Detail View object
     *
     */
    AccountDetailView::~AccountDetailView() = default;

    /**
     * @brief Setup the UI elements for the account detail view
     *
     */
    void AccountDetailView::_setupUi()
    {
        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        setLayout(layout);

        // Add UI elements to the layout
        _uiElements             = std::make_unique<UIElements>();
        _uiElements->titleLabel = utils::makeQChild<QLabel>("Account Details");
        _uiElements->nameLabel  = utils::makeQChild<QLabel>();
        _uiElements->balanceLabel = utils::makeQChild<QLabel>();

        layout->addWidget(_uiElements->titleLabel);
        layout->addWidget(_uiElements->nameLabel);
        layout->addWidget(_uiElements->balanceLabel);
    }

    /**
     * @brief Update the account details displayed in the view
     *
     * @param account The account data to display
     */
    void AccountDetailView::updateAccount(const AccountDraft& account)
    {
        _account = account;
        // Update the UI with account details

        // Update the UI elements with the new account details
        _uiElements->nameLabel->setText(
            "Name: " + QString::fromStdString(_account.name)
        );
        _uiElements->balanceLabel->setText(
            "Balance: " + QString::number(0) + " " +
            QString::fromStdString(CurrencyMeta::toString(_account.currency))
        );
    }

}   // namespace ui
