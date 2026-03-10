#ifndef __UTILS__VERSION_HPP__
#define __UTILS__VERSION_HPP__

#include <cstddef>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

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

        static SemVer getInvalidVersion();

        friend bool operator==(const SemVer& lhs, const SemVer& rhs);

       private:
        static std::optional<SemVer> _parse(const std::string& versionStr);
    };

}   // namespace utils

// cppcheck-suppress unknownMacro -- false positive
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
     * @param jsonData
     * @param version
     */
    static void to_json(nlohmann::json& jsonData, const utils::SemVer& version)
    {
        jsonData = version.toString();
    }

    /**
     * @brief Deserialize SemVer from JSON
     *
     * @param jsonData
     * @return utils::SemVer
     */
    static utils::SemVer from_json(const nlohmann::json& jsonData)
    {
        return utils::SemVer(jsonData.get<std::string>());
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __UTILS__VERSION_HPP__