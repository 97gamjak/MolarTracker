#include "profile_mapper.hpp"

#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"

namespace controller
{
    /**
     * @brief Convert a Profile to a ProfileDraft
     *
     * @param profile
     * @return drafts::ProfileDraft
     */
    drafts::ProfileDraft ProfileMapper::toDraft(const domain::Profile& profile)
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
    domain::Profile ProfileMapper::toProfile(const drafts::ProfileDraft& draft)
    {
        return domain::Profile{
            draft.getId(),
            draft.getName(),
            draft.getEmail()
        };
    }

}   // namespace controller
