#ifndef __STORE__SRC__STORE__ACCOUNT__ACCOUNT_MAPPER_HPP__
#define __STORE__SRC__STORE__ACCOUNT__ACCOUNT_MAPPER_HPP__

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace domain
{
    class Account;   // Forward declaration
}   // namespace domain

namespace store
{

    /**
     * @brief Maps a finance account to a domain account.
     *
     */
    class AccountMapper
    {
       public:
        [[nodiscard]]
        static domain::Account toDomain(const finance::Account& account);

        [[nodiscard]]
        static finance::Account fromDomain(const domain::Account& domain);
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__ACCOUNT__ACCOUNT_MAPPER_HPP__
