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

    void AccountCombo::resetSelection() { _combo->setCurrentIndex(-1); }

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