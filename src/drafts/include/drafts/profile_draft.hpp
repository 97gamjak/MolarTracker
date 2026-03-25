#ifndef __DRAFTS__PROFILE_DRAFT_HPP__
#define __DRAFTS__PROFILE_DRAFT_HPP__

#include <optional>
#include <string>

namespace drafts
{
    /**
     * @brief A draft representation of a user profile
     *
     */
    struct ProfileDraft
    {
        /// The name of the profile (required)
        std::string name;

        /// The email associated with the profile (optional)
        std::optional<std::string> email = std::nullopt;
    };
}   // namespace drafts

#endif   // __DRAFTS__PROFILE_DRAFT_HPP__