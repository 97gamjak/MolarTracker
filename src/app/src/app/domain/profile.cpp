#include "app/domain/profile.hpp"

#include <utility>

namespace app
{
    /**
     * @brief Construct a new Profile:: Profile object
     *
     * @param id
     * @param name
     * @param email
     */
    Profile::Profile(
        ProfileId                         id,
        std::string                       name,
        const std::optional<std::string>& email
    )
        : _id{id}, _name{std::move(name)}, _email{email}
    {
    }

    /**
     * @brief Get the ID of the profile
     *
     * @return ProfileId
     */
    ProfileId Profile::getId() const { return _id; }

    /**
     * @brief Get the name of the profile
     *
     * @return std::string
     */
    const std::string& Profile::getName() const { return _name; }

    /**
     * @brief Get the email of the profile
     *
     * @return std::optional<std::string>
     */
    const std::optional<std::string>& Profile::getEmail() const
    {
        return _email;
    }

    /**
     * @brief Set the ID of the profile
     *
     * @param newId
     */
    void Profile::setId(ProfileId newId) { _id = newId; }

    /**
     * @brief Set the name of the profile
     *
     * @param newName
     */
    void Profile::setName(const std::string& newName) { _name = newName; }

    /**
     * @brief Set the email of the profile
     *
     * @param newEmail
     */
    void Profile::setEmail(const std::optional<std::string>& newEmail)
    {
        _email = newEmail;
    }

}   // namespace app