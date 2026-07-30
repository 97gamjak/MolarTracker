#include "drafts/profile_draft.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Profile Draft:: Profile Draft object
     *
     * @param id
     * @param name
     * @param email
     */
    ProfileDraft::ProfileDraft(
        ProfileId                  id,
        std::string                name,
        std::optional<std::string> email
    )
        : _id(id), _name(std::move(name)), _email(std::move(email))
    {
    }

    /**
     * @brief Get the ID of the profile
     *
     * @return ProfileId
     */
    ProfileId ProfileDraft::getId() const { return _id; }

    /**
     * @brief Get the name of the profile
     *
     * @return const std::string&
     */
    const std::string& ProfileDraft::getName() const { return _name; }

    /**
     * @brief Get the email of the profile
     *
     * @return const std::optional<std::string>&
     */
    const std::optional<std::string>& ProfileDraft::getEmail() const
    {
        return _email;
    }
}   // namespace drafts