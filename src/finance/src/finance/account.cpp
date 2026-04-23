#include "finance/account.hpp"

#include <utility>

#include "config/finance.hpp"

namespace finance
{

    /**
     * @brief Construct a new Account:: Account object
     *
     * @param id the unique identifier for the cash account
     * @param status the status of the account (e.g., Active, Closed)
     * @param currency the current currency of the account
     * @param name the current name of the account
     * @param kind the kind of the account (e.g., Cash, External, Security)
     */
    Account::Account(
        AccountId     id,
        AccountStatus status,
        std::string   name,
        Currency      currency,
        AccountKind   kind
    )
        : _id(id), _status(status), _name(std::move(name)), _currency(currency)
    {
        switch (kind)
        {
            case AccountKind::Cash:
            {
                _details = CashAccount{};
                break;
            }
            case AccountKind::Security:
            {
                _details = SecurityAccount{};
                break;
            }
            case AccountKind::External:
            {
                _details = ExternalAccount{};
                break;
            }
        }
    }

    /**
     * @brief set the id of the account
     *
     * @param id
     */
    void Account::setId(AccountId id) { _id = id; }

    /**
     * @brief Get the Id of the cash account
     *
     * @return AccountId
     */
    AccountId Account::getId() const { return _id; }

    /**
     * @brief Get the status of the cash account
     *
     * @return AccountStatus
     */
    AccountStatus Account::getStatus() const { return _status; }

    /**
     * @brief Get the currency of the cash account
     *
     * @return Currency
     */
    Currency Account::getCurrency() const { return _currency; }

    /**
     * @brief Get the name of the cash account
     *
     * @return std::string
     */
    std::string Account::getName() const { return _name; }

    /**
     * @brief Get the kind of the account
     *
     * @return AccountKind
     */
    AccountKind Account::getKind() const
    {
        struct Visitor
        {
            AccountKind operator()(const CashAccount& /*details*/) const
            {
                return AccountKind::Cash;
            }
            AccountKind operator()(const ExternalAccount& /*details*/) const
            {
                return AccountKind::External;
            }
            AccountKind operator()(const SecurityAccount& /*details*/) const
            {
                return AccountKind::Security;
            }
        };
        return std::visit(Visitor{}, _details);
    }

    /**
     * @brief Check if the account is an external account
     *
     * @return true if the account is an external account, false otherwise
     */
    bool Account::isExternal() const
    {
        return getKind() == AccountKind::External;
    }

    filter::Predicate<Account> IsAccountType(AccountKind kind)
    {
        return filter::makePredicate<Account>(
            [kind](const Account& account) { return account.getKind() == kind; }
        );
    }

    filter::Predicate<Account> IsAccountActive()
    {
        return filter::makePredicate<Account>(
            [](const Account& account)
            { return account.getStatus() != AccountStatus::Closed; }
        );
    }

    filter::Predicate<Account> IsExternal()
    {
        return filter::makePredicate<Account>([](const Account& account)
                                              { return account.isExternal(); });
    }

    filter::Predicate<Account> HasCurrency(Currency currency)
    {
        return filter::makePredicate<Account>(
            [currency](const Account& account)
            { return account.getCurrency() == currency; }
        );
    }

    filter::Predicate<Account> HasAccountId(AccountId id)
    {
        return filter::makePredicate<Account>([id](const Account& account)
                                              { return account.getId() == id; }
        );
    }

}   // namespace finance