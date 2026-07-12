#ifndef __UI__INCLUDE__UI__TRANSACTION__OPTION_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__OPTION_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "ui/base/dialog.hpp"
#include "utils/container/set.hpp"

class QFormLayout;   // Forward declaration
class QLabel;        // Forward declaration
class QPushButton;   // Forward declaration

namespace drafts
{
    class CreateOptionTransactionDraft;   // Forward declaration
    class AccountDraft;                   // Forward declaration
}   // namespace drafts

namespace ui
{

    class AccountCombo;     // Forward declaration
    class AmountRow;        // Forward declaration
    class TickerField;      // Forward declaration
    class TimestampField;   // Forward declaration
    class CommentField;     // Forward declaration

    /**
     * @brief Widget for creating and editing option transactions, this widget
     * provides a user interface for users to input the necessary information to
     * create or edit option transactions, such as selecting the accounts
     * involved, entering the amount, selecting the ticker, and specifying the
     * timestamp and any optional comments, allowing for a user-friendly way to
     * manage option transactions within the application.
     *
     */
    class OptionWidget : public Dialog
    {
        Q_OBJECT
       private:
        /// The layout for this widget
        QFormLayout* _layout;

        /// The button for adding the transaction
        QPushButton* _addButton;

        struct Fields;
        /// Pointer to the fields struct
        std::unique_ptr<Fields> _fields;

        /// The list of reference accounts
        std::vector<drafts::AccountDraft> _referenceAccounts;

       public:
        explicit OptionWidget(
            const std::vector<drafts::AccountDraft>& accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            const Set<std::string>&                  tickers,
            QWidget*                                 parent = nullptr
        );

        ~OptionWidget() override;

        void updateAccounts(std::vector<drafts::AccountDraft> accounts);
        void updateReferenceAccounts(
            std::vector<drafts::AccountDraft> referenceAccounts
        );
        void updateTickers(const Set<std::string>& tickers);
        void refresh();

       signals:
        /**
         * @brief Emitted when a new ticker is requested
         *
         * @param ticker The ticker symbol to create
         */
        void createTickerRequested(const std::string& ticker);

        /**
         * @brief Emitted when a new option transaction is requested
         *
         * @param draft The draft of the option transaction to create
         */
        void createOptionTransactionRequested(
            const drafts::CreateOptionTransactionDraft& draft
        );

       private:
        void _onAccountSelected(const drafts::AccountDraft& account);
        void _onReferenceAccountSelected(const drafts::AccountDraft& account);
        void _updateAddButton();
        void _emitOk();

        [[nodiscard]]
        drafts::CreateOptionTransactionDraft _getDraft() const;

        void _connectAddButton();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__OPTION_WIDGET_HPP__