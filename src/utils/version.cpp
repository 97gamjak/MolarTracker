#include "version.hpp"

#include <stdexcept>

namespace utils
{
    SemVer::SemVer(int major, int minor, int patch)
        : _major(major), _minor(minor), _patch(patch)
    {
    }

    SemVer::SemVer(const std::string_view versionStr)
    {
        size_t firstDot  = versionStr.find('.');
        size_t secondDot = versionStr.find('.', firstDot + 1);

        if (firstDot == std::string::npos || secondDot == std::string::npos)
            throw std::invalid_argument(
                "Invalid version string: " + std::string(versionStr)
            );

        try
        {
            _major = std::stoi(std::string(versionStr.substr(0, firstDot)));
            _minor = std::stoi(
                std::string(
                    versionStr.substr(firstDot + 1, secondDot - firstDot - 1)
                )
            );
            _patch = std::stoi(std::string(versionStr.substr(secondDot + 1)));
        }
        catch (const std::exception& e)
        {
            throw std::invalid_argument(
                "Invalid version string: " + std::string(versionStr) + " (" +
                e.what() + ")"
            );
        }
    }
}   // namespace utils