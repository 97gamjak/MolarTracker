#include "domain/profile.hpp"

#include <utility>

namespace domain
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

    std::string Profile::toString() const
    {
        std::string emailStr = _email.has_value() ? _email.value() : "N/A";
        return std::format(
            "Profile[ID: {}, Name: {}, Email: {}]",
            _id.toString(),
            _name,
            emailStr
        );
    }

    /**
     * @brief Create a predicate to filter profiles by their ID
     *
     * @param id
     * @return filter::Predicate<Profile>
     */
    filter::Predicate<Profile> HasProfileId(ProfileId id)
    {
        return filter::makePredicate<Profile>([id](const Profile& profile)
                                              { return profile.getId() == id; }
        );
    }

    /**
     * @brief Create a predicate to filter profiles by their name
     *
     * @param name
     * @return filter::Predicate<Profile>
     */
    filter::Predicate<Profile> HasProfileName(std::string_view name)
    {
        return filter::makePredicate<Profile>(
            [name](const Profile& profile) { return profile.getName() == name; }
        );
    }

}   // namespace domain