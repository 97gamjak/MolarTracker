// account_combo.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__ACCOUNT_COMBO_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__ACCOUNT_COMBO_HPP__

#include <qwidget.h>

#include <optional>
#include <vector>

#include "drafts/account_draft.hpp"

class QComboBox;   // Forward declaration

namespace ui
{

    /**
     * @brief Reusable account selection combo box widget
     *
     * Wraps a QComboBox populated with a list of AccountDrafts and exposes
     * a typed signal and accessor so owning widgets never have to deal with
     * raw QVariant / index lookups themselves.
     */
    class AccountCombo : public QWidget
    {
        Q_OBJECT

       public:
        explicit AccountCombo(
            std::vector<drafts::AccountDraft> accounts,
            QWidget*                          parent = nullptr
        );

        [[nodiscard]] std::optional<drafts::AccountDraft> selected() const;

        void resetSelection();
        void updateAccounts(std::vector<drafts::AccountDraft> accounts);

       signals:
        /**
         * @brief Emitted when the user selects an account from the combo box
         *
         * @param account The selected account draft
         */
        void accountSelected(drafts::AccountDraft account);

       private:
        std::vector<drafts::AccountDraft> _accounts;
        QComboBox*                        _combo;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__ACCOUNT_COMBO_HPP__