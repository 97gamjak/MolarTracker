#include "version.hpp"

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
        std::vector<std::size_t> values;
        std::string              valueStr;

        for (const auto& character : versionStr)
        {
            if (character != '.' && character != '+' &&
                !static_cast<bool>(std::isdigit(character)))
                return std::nullopt;

            if (character == '+' || character == '.')
            {
                if (valueStr.empty())
                    return std::nullopt;

                int value = 0;
                try
                {
                    value = std::stoi(valueStr);
                    if (value < 0)
                        return std::nullopt;

                    values.push_back(static_cast<std::size_t>(value));
                }
                catch (const std::exception&)
                {
                    return std::nullopt;
                }
            }
            else
            {
                valueStr += character;
                continue;
            }

            if (character == '+')
                break;
        }

        if (values.size() != 3)
            return std::nullopt;

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
     * @brief Get an invalid SemVer object, this can be used to represent an
     * invalid version when parsing fails, this is useful for error handling
     *
     * @return SemVer
     */
    SemVer SemVer::getInvalidVersion() { return SemVer{"invalid"}; }

    /**
     * @brief Equality operator for SemVer, two SemVer objects are
     * considered equal if their major, minor, and patch numbers are all
     * equal, if both SemVer objects are invalid, they are also considered
     * equal
     *
     * @param lhs
     * @param rhs
     * @return true if the two SemVer objects are equal, false otherwise
     */
    bool operator==(const SemVer& lhs, const SemVer& rhs)
    {
        if (lhs._isInvalid != rhs._isInvalid)
            return false;

        if (lhs._isInvalid && rhs._isInvalid)
            return true;

        return lhs._major == rhs._major && lhs._minor == rhs._minor &&
               lhs._patch == rhs._patch;
    }
}   // namespace utils