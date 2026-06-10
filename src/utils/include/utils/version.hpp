#ifndef __UTILS__INCLUDE__UTILS__VERSION_HPP__
#define __UTILS__INCLUDE__UTILS__VERSION_HPP__

#include <compare>
#include <cstddef>
#include <optional>
#include <string>

class QString;   // Forward declaration

namespace utils
{

    /**
     * @brief Semantic Versioning class
     */
    class SemVer
    {
       private:
        /// The major version number
        std::size_t _major;
        /// The minor version number
        std::size_t _minor;
        /// The patch version number
        std::size_t _patch;

        /// Flag indicating whether the version is invalid
        bool _isInvalid = false;

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
        [[nodiscard]] QString     toQString() const;

        static SemVer getInvalidVersion();

        // Returns the compile-time version of the running application.
        static SemVer current();

        friend bool operator==(const SemVer& lhs, const SemVer& rhs);
        friend std::strong_ordering operator<=>(
            const SemVer& lhs,
            const SemVer& rhs
        );

       private:
        static std::optional<SemVer> _parse(const std::string& versionStr);
    };

}   // namespace utils

#endif   // __UTILS__INCLUDE__UTILS__VERSION_HPP__