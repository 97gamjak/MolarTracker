#include "app/store/predicates.hpp"

namespace app
{
    /**
     * @brief Create a predicate that checks if an entry has a specific account
     * ID
     *
     * @param id The account ID to check for
     * @return Predicate<finance::Account>
     */
    Predicate<finance::Account> HasAccountId(AccountId id)
    {
        return Predicate<finance::Account>([id](const auto& entry)
                                           { return getId(entry) == id; });
    }

    /**
     * @brief Create a predicate that checks if an entry is an external account
     *
     * @return Predicate<finance::Account>
     */
    Predicate<finance::Account> IsExternal()
    {
        return Predicate<finance::Account>([](const auto& entry)
                                           { return entry.isExternal(); });
    }

    /**
     * @brief Create a predicate that checks if an entry has a specific currency
     *
     * @return Predicate<finance::Account>
     */
    Predicate<finance::Account> HasCurrency(Currency currency)
    {
        return Predicate<finance::Account>(
            [currency](const auto& entry)
            { return entry.getCurrency() == currency; }
        );
    }

}   // namespace app