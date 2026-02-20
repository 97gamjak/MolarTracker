#ifndef __UTILS__VERSION_HPP__
#define __UTILS__VERSION_HPP__

#include <optional>
#include <string>

#include "nlohmann/json.hpp"

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

        static const SemVer getInvalidVersion();

        friend bool operator==(const SemVer& lhs, const SemVer& rhs);

       private:
        std::optional<SemVer> _parse(const std::string& versionStr);
    };

}   // namespace utils

NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for utils::SemVer
 *
 */
template <>
struct adl_serializer<utils::SemVer>
{
    /**
     * @brief Serialize SemVer to JSON
     *
     * @param j
     * @param version
     */
    static void to_json(nlohmann::json& j, const utils::SemVer& version)
    {
        j = version.toString();
    }

    /**
     * @brief Deserialize SemVer from JSON
     *
     * @param j
     * @return utils::SemVer
     */
    static utils::SemVer from_json(const nlohmann::json& j)
    {
        return utils::SemVer(j.get<std::string>());
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __UTILS__VERSION_HPP__