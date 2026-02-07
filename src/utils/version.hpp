#ifndef __UTILS__VERSION_HPP__
#define __UTILS__VERSION_HPP__

#include <string_view>

namespace utils
{
    /**
     * @brief Struct representing a semantic version (major.minor.patch)
     *
     */
    class SemVer
    {
       public:
        SemVer() = default;
        SemVer(int major, int minor, int patch);
        SemVer(const std::string_view versionStr);

        int _major = 0;
        int _minor = 0;
        int _patch = 0;

        friend auto operator<=>(const SemVer&, const SemVer&) = default;
    };
}   // namespace utils

#endif   // __UTILS__VERSION_HPP__