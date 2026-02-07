#ifndef __UTILS__VERSION_HPP__
#define __UTILS__VERSION_HPP__

#include <string>

namespace utils
{

    /**
     * @brief Semantic Versioning class
     */
    class SemVer
    {
       private:
        std::size_t _major;
        std::size_t _minor;
        std::size_t _patch;

       public:
        SemVer() = delete;
        explicit SemVer(
            std::size_t major,
            std::size_t minor,
            std::size_t patch
        );
        explicit SemVer(const std::string& versionStr);

        [[nodiscard]] std::size_t getMajor() const;
        [[nodiscard]] std::size_t getMinor() const;
        [[nodiscard]] std::size_t getPatch() const;

        [[nodiscard]] std::string toString() const;
    };

}   // namespace utils

#endif   // __UTILS__VERSION_HPP__