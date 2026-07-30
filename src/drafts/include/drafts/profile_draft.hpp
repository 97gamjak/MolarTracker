#ifndef __DRAFTS__INCLUDE__DRAFTS__PROFILE_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__PROFILE_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"

namespace drafts
{
    /**
     * @brief A draft representation of a user profile
     *
     */
    class ProfileDraft
    {
        /// The ID of the profile (required)
        ProfileId _id;

        /// The name of the profile (required)
        std::string _name;

        /// The email associated with the profile (optional)
        std::optional<std::string> _email = std::nullopt;

       public:
        explicit ProfileDraft(
            ProfileId                  id,
            std::string                name,
            std::optional<std::string> email = std::nullopt
        );

        [[nodiscard]]
        ProfileId getId() const;

        [[nodiscard]]
        const std::string& getName() const;

        [[nodiscard]]
        const std::optional<std::string>& getEmail() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__PROFILE_DRAFT_HPP__