#include "app/store/profile/predicates.hpp"

namespace app
{
    /**
     * @brief Predicate for finding a profile by its ID
     *
     */
    Predicate<Profile> HasProfileId(ProfileId id)
    {
        return Predicate<Profile>{[id](const auto& entry)
                                  { return entry.getId() == id; }};
    }

    /**
     * @brief Predicate for finding a profile by its name
     *
     */
    Predicate<Profile> HasProfileName(std::string_view name)
    {
        return Predicate<Profile>{[name](const auto& entry)
                                  { return entry.getName() == name; }};
    }

}   // namespace app