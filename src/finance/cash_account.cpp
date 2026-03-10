#include "cash_account.hpp"

#include <utility>

namespace finance
{

    /**
     * @brief Construct a new CashAccount:: CashAccount object
     *
     * @param id the unique identifier for the cash account
     * @param status the status of the account (e.g., Active, Closed)
     * @param profileId the profile ID associated with the account
     * @param currency the current currency of the account
     * @param name the current name of the account
     */
    CashAccount::CashAccount(
        AccountId     id,
        AccountStatus status,
        ProfileId     profileId,
        Currency      currency,
        std::string   name
    )
        : _id(id),
          _status(status),
          _profileId(profileId),
          _currency(currency),
          _name(std::move(name))
    {
    }

    /**
     * @brief Get the Id of the cash account
     *
     * @return AccountId
     */
    AccountId CashAccount::getId() const { return _id; }

    /**
     * @brief Get the status of the cash account
     *
     * @return AccountStatus
     */
    AccountStatus CashAccount::getStatus() const { return _status; }

    /**
     * @brief Get the profile ID associated with the cash account
     *
     * @return ProfileId
     */
    ProfileId CashAccount::getProfileId() const { return _profileId; }

    /**
     * @brief Get the currency of the cash account
     *
     * @return Currency
     */
    Currency CashAccount::getCurrency() const { return _currency; }

    /**
     * @brief Get the name of the cash account
     *
     * @return std::string
     */
    std::string CashAccount::getName() const { return _name; }

}   // namespace finance