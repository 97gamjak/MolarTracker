#ifndef __DRAFTS__PROFILE_DRAFT_HPP__
#define __DRAFTS__PROFILE_DRAFT_HPP__

#include <optional>
#include <string>

namespace drafts
{
    struct ProfileDraft
    {
        std::string                name;
        std::optional<std::string> email = std::nullopt;
    };
}   // namespace drafts

#endif   // __DRAFTS__PROFILE_DRAFT_HPP__