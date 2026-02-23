#include "cash_account.hpp"

namespace finance
{

    /**
     * @brief Construct a new CashAccount:: CashAccount object
     *
     * @param id the unique identifier for the cash account
     * @param type the type of the account (e.g., Cash, Securities, External)
     * @param status the status of the account (e.g., Active, Closed)
     * @param profileId the profile ID associated with the account
     * @param currency the current currency of the account
     * @param name the current name of the account
     */
    CashAccount::CashAccount(
        AccountId     id,
        AccountType   type,
        AccountStatus status,
        ProfileId     profileId,
        Currency      currency,
        std::string   name
    )
        : _id(id),
          _type(type),
          _status(status),
          _profileId(profileId),
          _currency(currency),
          _name(std::move(name))
    {
    }

}   // namespace finance