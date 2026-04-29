// account_combo.cpp
#include "ui/transaction/account_combo.hpp"

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qvariant.h>

#include <algorithm>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Account Combo:: Account Combo object
     *
     * @param accounts A list of account drafts to populate the combo box with
     * @param parent The parent widget for the account combo
     */
    AccountCombo::AccountCombo(
        std::vector<drafts::AccountDraft> accounts,
        QWidget*                          parent
    )
        : QWidget(parent),
          _accounts(std::move(accounts)),
          _combo(makeQChild<QComboBox>(this))
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_combo);
        setLayout(layout);

        _combo->setPlaceholderText("Select Account");

        for (const auto& account : _accounts)
        {
            _combo->addItem(
                QString::fromStdString(account.name),
                QVariant::fromValue(account.id)
            );
        }

        connect(
            _combo,
            &QComboBox::activated,
            this,
            [this](int /*index*/)
            {
                const auto account = selected();
                if (account.has_value())
                    emit accountSelected(*account);
            }
        );
    }

    /**
     * @brief Get the currently selected account from the combo box, this will
     * return an optional containing the selected account draft if an account is
     * selected, or std::nullopt if no account is selected, allowing the owning
     * widget to easily check for a selection and handle it accordingly.
     *
     * @return std::optional<drafts::AccountDraft> An optional containing the
     * selected account draft if an account is selected, or std::nullopt if no
     * account is selected.
     */
    std::optional<drafts::AccountDraft> AccountCombo::selected() const
    {
        const auto id = _combo->currentData().value<AccountId>();

        const auto it = std::ranges::find_if(
            _accounts,
            [id](const drafts::AccountDraft& account)
            { return account.id == id; }
        );

        return it != _accounts.end() ? std::optional{*it} : std::nullopt;
    }

    /**
     * @brief Reset the selection in the combo box, this will clear any current
     * selection and set the combo box back to its placeholder state, allowing
     * the user to make a new selection if needed.
     *
     */
    void AccountCombo::resetSelection() { _combo->setCurrentIndex(-1); }

    /**
     * @brief Update the list of accounts in the combo box, this will replace
     * the current list of accounts with the provided list, and update the
     * combo box items accordingly, allowing the combo box to reflect any
     * changes to the available accounts.
     *
     * @param accounts A new list of account drafts to populate the combo box
     * with
     */
    void AccountCombo::updateAccounts(
        std::vector<drafts::AccountDraft> accounts
    )
    {
        _accounts = std::move(accounts);
        _combo->clear();

        for (const auto& account : _accounts)
        {
            _combo->addItem(
                QString::fromStdString(account.name),
                QVariant::fromValue(account.id)
            );
        }
    }

}   // namespace ui