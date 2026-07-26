#ifndef __MAPPER__INCLUDE__MAPPER__PROFILE_MAPPER_HPP__
#define __MAPPER__INCLUDE__MAPPER__PROFILE_MAPPER_HPP__

#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"

namespace mapper
{
    /**
     * @brief Mapper class for converting between Profile and ProfileDraft
     *
     */
    class ProfileMapper
    {
       public:
        [[nodiscard]]
        static drafts::ProfileDraft toDraft(const domain::Profile& profile);

        [[nodiscard]]
        static domain::Profile toProfile(const drafts::ProfileDraft& draft);
    };
}   // namespace mapper

#endif   // __MAPPER__INCLUDE__MAPPER__PROFILE_MAPPER_HPP__
