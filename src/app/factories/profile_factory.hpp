#ifndef __APP__FACTORIES__PROFILE_FACTORY_HPP__
#define __APP__FACTORIES__PROFILE_FACTORY_HPP__

#include <vector>

struct ProfileRow;   // Forward declaration

namespace app
{
    class Profile;   // Forward declaration

    /**
     * @brief Factory for creating Profile instances
     *
     */
    class ProfileFactory
    {
       public:
        static Profile              toDomain(const ProfileRow& row);
        static std::vector<Profile> toDomains(
            const std::vector<ProfileRow>& rows
        );

        static ProfileRow toRow(const Profile& profile);
    };

}   // namespace app

#endif   // __APP__FACTORIES__PROFILE_FACTORY_HPP__