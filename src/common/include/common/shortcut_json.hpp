#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__

#include <nlohmann/json.hpp>

#include "common/shortcut.hpp"

// cppcheck-suppress unknownMacro -- false positive
NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for Shortcut
 *
 */
template <>
struct adl_serializer<Shortcut>
{
    /**
     * @brief Serialize Shortcut to JSON
     *
     * @param jsonData
     * @param shortcut
     */
    static void to_json(nlohmann::json& jsonData, const Shortcut& shortcut)
    {
        jsonData["what"] = shortcut.getWhat();
        jsonData["modifiers"] =
            static_cast<std::int64_t>(shortcut.getModifiers());
        jsonData["key"] = shortcut.getKey();
    }

    /**
     * @brief Deserialize SemVer from JSON
     *
     * @param jsonData
     * @return Shortcut
     */
    static Shortcut from_json(const nlohmann::json& jsonData)
    {
        const auto what      = jsonData.at("what").get<std::string>();
        const auto modifiers = static_cast<ShortcutModifier>(
            jsonData.at("modifiers").get<std::int64_t>()
        );
        const auto key = jsonData.at("key").get<std::uint64_t>();

        return {what, modifiers, key};
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__