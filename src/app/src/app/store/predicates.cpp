#include "app/store/predicates.hpp"

namespace app
{
    Predicate<BaseStore<finance::Account, AccountId>::Entry> HasAccountId(
        AccountId id
    )
    {
        return Predicate<BaseStore<finance::Account, AccountId>::Entry>(
            [id](const auto& entry) { return getId(entry.value) == id; }
        );
    }

}   // namespace app
