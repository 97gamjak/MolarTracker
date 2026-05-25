#ifndef __STORE__INCLUDE__STORE__I_PROFILE_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_PROFILE_STORE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "config/signal_tags.hpp"

class Connection;

namespace drafts
{
    class ProfileDraft;   // Forward declaration
}   // namespace drafts

namespace domain
{
    class Profile;   // Forward declaration
}   // namespace domain

namespace store
{

    /**
     * @brief Result of profile store operations
     *
     */
    enum class ProfileStoreResult : std::uint8_t;   // for doxygen

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define PROFILE_STORE_RESULT_LIST(XX) \
    XX(Ok)                            \
    XX(Error)                         \
    XX(NameAlreadyExists)             \
    XX(ProfileNotFound)

    // TODO(97gamjak): make return strategy via custom result type
    MSTD_ENUM(ProfileStoreResult, std::uint8_t, PROFILE_STORE_RESULT_LIST);

    /**
     * @brief Store for managing profiles
     *
     */
    class IProfileStore
    {
       public:
        virtual ~IProfileStore() = default;

        /**
         * @brief checks if a profile exists
         *
         * @return true if the profile exists
         * @return false if the profile does not exist
         */
        [[nodiscard]]
        virtual bool hasProfiles() const = 0;

        /**
         * @brief Check if a profile exists
         *
         * @param name The name of the profile to check
         * @return true if the profile exists
         * @return false if the profile does not exist
         */
        [[nodiscard]]
        virtual bool profileExists(std::string_view name) const = 0;

        /**
         * @brief Get all profile names
         *
         * @return std::vector<std::string> containing all profile names
         */
        [[nodiscard]]
        virtual std::vector<std::string> getAllProfileNames() const = 0;

        /**
         * @brief Set the Active Profile
         *
         * @param name
         * @return ProfileStoreResult
         */
        [[nodiscard]]
        virtual ProfileStoreResult setActiveProfile(std::string_view name) = 0;

        /**
         * @brief Get the Active Profile
         *
         * @return std::optional<drafts::ProfileDraft> containing the active
         * profile, if it exists
         */
        [[nodiscard]]
        virtual std::optional<drafts::ProfileDraft> getActiveProfile(
        ) const = 0;

        /**
         * @brief Add a profile
         *
         * @param draft The profile draft to add
         * @return ProfileStoreResult indicating the result of the operation
         */
        [[nodiscard]]
        virtual ProfileStoreResult addProfile(
            const drafts::ProfileDraft& draft
        ) = 0;

        /**
         * @brief Remove a profile
         *
         * @param draft The profile draft to remove
         * @return ProfileStoreResult indicating the result of the operation
         */
        [[nodiscard]]
        virtual ProfileStoreResult removeProfile(
            const drafts::ProfileDraft& draft
        ) = 0;

        /**
         * @brief Commit the current changes
         *
         */
        virtual void commit() = 0;

        /**
         * @brief Subscribe to profile changes
         *
         * @param func The callback function to call on profile changes
         * @param user User data to pass to the callback
         * @return Connection object for managing the subscription
         */
        [[nodiscard]]
        virtual Connection subscribeToProfileChange(
            const OnProfileChanged::func& func,
            void*                         user
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_PROFILE_STORE_HPP__