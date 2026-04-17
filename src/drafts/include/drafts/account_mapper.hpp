#ifndef __DRAFTS_ACCOUNT_MAPPER_HPP__
#define __DRAFTS_ACCOUNT_MAPPER_HPP__

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace drafts
{
    struct AccountDraft;   // Forward declaration

    class AccountMapper
    {
       public:
        static AccountDraft     toDraft(const finance::Account& account);
        static finance::Account toAccount(const AccountDraft& draft);
    };

}   // namespace drafts

#endif   // __DRAFTS_ACCOUNT_MAPPER_HPP__
