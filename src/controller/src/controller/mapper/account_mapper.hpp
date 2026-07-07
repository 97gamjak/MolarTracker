#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__

#include <vector>

#include "drafts/account_draft.hpp"   // needed for vector of accoutndrafts

namespace finance
{
    class Account;        // Forward declaration
    class AccountsView;   // Forward declaration
}   // namespace finance

namespace controller
{
    /**
     * @brief Mapper class for converting between Account and AccountDraft
     *
     * This class provides static methods to convert between the domain model
     * (finance::Account) and the draft model (drafts::AccountDraft). This is
     * useful for separating the concerns of the business logic and the UI, and
     * allows us to have a clear mapping between the two representations of an
     * account.
     */
    class AccountMapper
    {
       public:
        [[nodiscard]]
        static drafts::AccountDraft toDraft(const finance::Account& account);

        [[nodiscard]]
        static std::vector<drafts::AccountDraft> toDrafts(
            const finance::AccountsView& accounts
        );

        [[nodiscard]]
        static finance::Account toAccount(const drafts::AccountDraft& draft);
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__
