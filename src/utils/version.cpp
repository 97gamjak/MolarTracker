#include "version.hpp"

#include <charconv>

namespace utils
{
    /**
     * @brief Construct a new Sem Ver:: Sem Ver object
     *
     * @param major
     * @param minor
     * @param patch
     */
    SemVer::SemVer(std::size_t major, std::size_t minor, std::size_t patch)
        : _major(major), _minor(minor), _patch(patch)
    {
    }

    /**
     * @brief Construct a new Sem Ver:: Sem Ver object
     *
     * @param versionStr
     */
    SemVer::SemVer(const std::string& versionStr)
    {
        auto parsedVersion = _parse(versionStr);
        if (parsedVersion.has_value())
            *this = *parsedVersion;
        else
        {
            _isInvalid = true;
            _major     = 0;
            _minor     = 0;
            _patch     = 0;
        }
    }

    /**
     * @brief Parse a version string into a SemVer object
     *
     * @param versionStr
     * @return std::optional<SemVer>
     */
    std::optional<SemVer> SemVer::_parse(const std::string& versionStr)
    {
        std::size_t values[3]{};
        std::size_t idx = 0;

        const char* first = versionStr.data();
        const char* last  = first + versionStr.size();

        while (idx < 3)
        {
            auto [ptr, ec] = std::from_chars(first, last, values[idx]);
            if (ec != std::errc{})
                return std::nullopt;

            first = ptr;

            if (idx < 2)
            {
                if (first == last || *first != '.')
                    return std::nullopt;
                ++first;
            }

            ++idx;
        }

        // Allow SemVer build metadata: "+anything"
        if (first != last)
        {
            if (*first != '+')
                return std::nullopt;
            // everything after '+' is ignored
        }

        return SemVer(values[0], values[1], values[2]);
    }

    /**
     * @brief Get the Major version number
     *
     * @return std::size_t
     */
    std::size_t SemVer::getMajor() const { return _major; }

    /**
     * @brief Get the Minor version number
     *
     * @return std::size_t
     */
    std::size_t SemVer::getMinor() const { return _minor; }

    /**
     * @brief Get the Patch version number
     *
     * @return std::size_t
     */
    std::size_t SemVer::getPatch() const { return _patch; }

    /**
     * @brief Convert the SemVer object to a string
     *
     * @return std::string
     */
    std::string SemVer::toString() const
    {
        return std::to_string(_major) + "." + std::to_string(_minor) + "." +
               std::to_string(_patch);
    }

    /**
     * @brief Equality operator for SemVer, two SemVer objects are considered
     * equal if their major, minor, and patch numbers are all equal, if both
     * SemVer objects are invalid, they are also considered equal
     *
     * @param lhs
     * @param rhs
     * @return true if the two SemVer objects are equal, false otherwise
     */
    bool operator==(const SemVer& lhs, const SemVer& rhs)
    {
        if (lhs._isInvalid && rhs._isInvalid)
            return true;

        return lhs._major == rhs._major && lhs._minor == rhs._minor &&
               lhs._patch == rhs._patch;
    }
}   // namespace utils