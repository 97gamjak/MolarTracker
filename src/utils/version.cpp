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
        sscanf(versionStr.c_str(), "%zu.%zu.%zu", &_major, &_minor, &_patch);
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
}   // namespace utils