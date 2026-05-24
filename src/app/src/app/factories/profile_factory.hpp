#ifndef __APP__SRC__APP__FACTORIES__PROFILE_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__PROFILE_FACTORY_HPP__

#include <vector>

struct ProfileRow;   // Forward declaration

namespace domain
{
    class Profile;   // Forward declaration
}   // namespace domain

namespace app
{

    /**
     * @brief Factory for creating Profile instances
     *
     */
    class ProfileFactory
    {
       public:
        [[nodiscard]]
        static domain::Profile toDomain(const ProfileRow& row);

        [[nodiscard]]
        static std::vector<domain::Profile> toDomains(
            const std::vector<ProfileRow>& rows
        );

        [[nodiscard]]
        static ProfileRow toRow(const domain::Profile& profile);
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__PROFILE_FACTORY_HPP__