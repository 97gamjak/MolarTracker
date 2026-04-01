#include "finance/account.hpp"

#include <utility>

namespace finance
{

    /**
     * @brief Construct a new Account:: Account object
     *
     * @param id the unique identifier for the cash account
     * @param status the status of the account (e.g., Active, Closed)
     * @param currency the current currency of the account
     * @param name the current name of the account
     */
    Account::Account(
        AccountId     id,
        AccountStatus status,
        std::string   name,
        Currency      currency
    )
        : _id(id), _status(status), _name(std::move(name)), _currency(currency)
    {
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

}   // namespace finance