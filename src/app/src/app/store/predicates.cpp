#include "app/store/predicates.hpp"

namespace app
{
    Predicate<BaseStore<finance::AccountVariant, AccountId>::Entry> HasAccountId(
        AccountId id
    )
    {
        return Predicate<BaseStore<finance::AccountVariant, AccountId>::Entry>(
            [id](const auto& entry) { return getId(entry.value) == id; }
        );
    }

}   // namespace app
