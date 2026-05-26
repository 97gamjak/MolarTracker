#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__

#include <vector>

// needed to not include it everywhere else
#include "drafts/account/account_draft.hpp"
#include "logic/finance/account.hpp"

namespace finance
{
    class Account;   // Forward declaration
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
            const std::vector<finance::Account>& accounts
        );

        [[nodiscard]]
        static finance::Account fromDraft(const drafts::AccountDraft& draft);
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__ACCOUNT_MAPPER_HPP__
