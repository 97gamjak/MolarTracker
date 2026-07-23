#ifndef __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__
#define __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__

#include <nlohmann/json.hpp>

#include "common/version.hpp"

// cppcheck-suppress unknownMacro -- false positive
NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for common::SemVer
 *
 */
template <>
struct adl_serializer<common::SemVer>
{
    /**
     * @brief Serialize SemVer to JSON
     *
     * @param jsonData
     * @param version
     */
    static void to_json(nlohmann::json& jsonData, const common::SemVer& version)
    {
        jsonData = version.toString();
    }

    /**
     * @brief Deserialize SemVer from JSON
     *
     * @param jsonData
     * @return common::SemVer
     */
    static common::SemVer from_json(const nlohmann::json& jsonData)
    {
        return common::SemVer(jsonData.get<std::string>());
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__