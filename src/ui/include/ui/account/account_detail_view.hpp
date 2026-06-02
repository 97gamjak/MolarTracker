#ifndef __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__
#define __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__

#include <QWidget>
#include <memory>

namespace drafts
{
    class PositionStockDetailDraft;   // Forward declaration
    class AccountDraft;               // Forward declaration
}   // namespace drafts

namespace ui
{
    /**
     * @brief View for displaying account details
     *
     */
    class AccountDetailView : public QWidget
    {
        Q_OBJECT

       private:
        /// The account draft being displayed
        std::unique_ptr<drafts::AccountDraft> _account;

        struct UIElements;
        /// PIMPL
        std::unique_ptr<UIElements> _uiElements;

       public:
        explicit AccountDetailView(QWidget* parent);
        ~AccountDetailView() override;

        void updateCashAccount(const drafts::AccountDraft& account);
        void updateSecurityAccount(
            const drafts::AccountDraft&                          account,
            const std::vector<drafts::PositionStockDetailDraft>& positions
        );

       private:
        void _updateAccount(const drafts::AccountDraft& account);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__
