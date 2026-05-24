#include "domain/profile_mapper.hpp"

#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"

namespace domain
{
    /**
     * @brief Convert a Profile to a ProfileDraft
     *
     * @param profile
     * @return drafts::ProfileDraft
     */
    drafts::ProfileDraft ProfileMapper::toDraft(const Profile& profile)
    {
        return drafts::ProfileDraft{
            profile.getId(),
            profile.getName(),
            profile.getEmail()
        };
    }

    /**
     * @brief Convert a ProfileDraft to a Profile
     *
     * @param draft
     * @return Profile
     */
    Profile ProfileMapper::toProfile(const drafts::ProfileDraft& draft)
    {
        return Profile{draft.getId(), draft.getName(), draft.getEmail()};
    }

}   // namespace domain
