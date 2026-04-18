#ifndef __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_MAPPER_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_MAPPER_HPP__

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace drafts
{
    struct AccountDraft;   // Forward declaration

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
        static AccountDraft     toDraft(const finance::Account& account);
        static finance::Account toAccount(const AccountDraft& draft);
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_MAPPER_HPP__
