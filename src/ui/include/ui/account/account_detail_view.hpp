#ifndef __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__
#define __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__

#include <QWidget>
#include <memory>

#include "drafts/account_draft.hpp"

namespace ui
{

    class AccountDetailView : public QWidget
    {
        Q_OBJECT

       private:
        drafts::AccountDraft _account;

        // PIMPL
        struct UIElements;
        std::unique_ptr<UIElements> _uiElements;

       public:
        explicit AccountDetailView(QWidget* parent);
        ~AccountDetailView() override;

        void updateAccount(const drafts::AccountDraft& account);

       private:
        void _setupUi();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__ACCOUNT__ACCOUNT_DETAIL_VIEW_HPP__
