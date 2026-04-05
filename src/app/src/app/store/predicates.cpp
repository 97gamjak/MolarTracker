#include "app/store/predicates.hpp"

namespace app
{
    /**
     * @brief Create a predicate that checks if an entry has a specific account
     * ID
     *
     * @param id The account ID to check for
     * @return Predicate<BaseStore<finance::Account, AccountId>::Entry>
     */
    Predicate<BaseStore<finance::Account, AccountId>::Entry> HasAccountId(
        AccountId id
    )
    {
        return Predicate<BaseStore<finance::Account, AccountId>::Entry>(
            [id](const auto& entry) { return getId(entry.value) == id; }
        );
    }

    /**
     * @brief Create a predicate that checks if an entry is an external account
     *
     * @return Predicate<BaseStore<finance::Account, AccountId>::Entry>
     */
    Predicate<BaseStore<finance::Account, AccountId>::Entry> IsExternal()
    {
        return Predicate<BaseStore<finance::Account, AccountId>::Entry>(
            [](const auto& entry) { return entry.value.isExternal(); }
        );
    }

    /**
     * @brief Create a predicate that checks if an entry has a specific currency
     *
     * @return Predicate<BaseStore<finance::Account, AccountId>::Entry>
     */
    Predicate<BaseStore<finance::Account, AccountId>::Entry> hasCurrency(
        Currency currency
    )
    {
        return Predicate<BaseStore<finance::Account, AccountId>::Entry>(
            [currency](const auto& entry)
            { return entry.value.getCurrency() == currency; }
        );
    }

}   // namespace app
