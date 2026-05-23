#ifndef __DOMAIN__INCLUDE__DOMAIN__PROFILE_MAPPER_HPP__
#define __DOMAIN__INCLUDE__DOMAIN__PROFILE_MAPPER_HPP__

namespace drafts
{
    class ProfileDraft;   // Forward declaration
}   // namespace drafts

namespace domain
{
    class Profile;

    /**
     * @brief Mapper class for converting between Profile and ProfileDraft
     *
     */
    class ProfileMapper
    {
       public:
        static drafts::ProfileDraft toDraft(const Profile& profile);
        static Profile toProfile(const drafts::ProfileDraft& draft);
    };
}   // namespace domain

#endif   // __DOMAIN__INCLUDE__DOMAIN__PROFILE_MAPPER_HPP__
