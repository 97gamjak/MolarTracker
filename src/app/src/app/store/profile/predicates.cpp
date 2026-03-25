#include "app/store/profile/predicates.hpp"

namespace app
{
    /**
     * @brief Predicate for finding a profile by its ID
     *
     */
    Predicate<BaseStore<Profile, ProfileId>::Entry> HasProfileId(ProfileId id)
    {
        return Predicate<BaseStore<Profile, ProfileId>::Entry>{
            [id](const auto& entry) { return entry.value.getId() == id; }
        };
    }

    /**
     * @brief Predicate for finding a profile by its name
     *
     */
    Predicate<BaseStore<Profile, ProfileId>::Entry> HasProfileName(
        std::string_view name
    )
    {
        return Predicate<BaseStore<Profile, ProfileId>::Entry>{
            [name](const auto& entry) { return entry.value.getName() == name; }
        };
    }

}   // namespace app