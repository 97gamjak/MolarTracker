#ifndef __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__
#define __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__

#include <nlohmann/json.hpp>

#include "utils/version.hpp"

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

#endif   // __UTILS__INCLUDE__UTILS__VERSION_JSON_HPP__