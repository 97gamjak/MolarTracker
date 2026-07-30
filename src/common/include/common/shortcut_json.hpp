#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__

#include <nlohmann/json.hpp>

#include "common/container/set.hpp"
#include "common/shortcut.hpp"
#include "common/shortcut_set.hpp"

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
        jsonData["modifiers"] =
            static_cast<std::int64_t>(shortcut.getModifiers());
        jsonData["key"] = shortcut.getKey();
    }

    /**
     * @brief Deserialize Shortcut from JSON
     *
     * @param jsonData
     * @return Shortcut
     */
    static Shortcut from_json(const nlohmann::json& jsonData)
    {
        const auto modifiers = static_cast<ShortcutModifier>(
            jsonData.at("modifiers").get<std::int64_t>()
        );
        const auto key = jsonData.at("key").get<std::uint64_t>();

        return {modifiers, key};
    }
};

/**
 * @brief Serializer for Set<T, Hash>
 *
 * @tparam T
 * @tparam Hash
 */
template <typename T, typename Hash>
struct adl_serializer<Set<T, Hash>>
{
    /**
     * @brief Serialize Set<T, Hash> to JSON
     *
     * @param jsonData
     * @param set
     */
    static void to_json(nlohmann::json& jsonData, const Set<T, Hash>& set)
    {
        jsonData = nlohmann::json::array();
        for (const auto& item : set)
        {
            jsonData.push_back(item);
        }
    }

    /**
     * @brief Deserialize Set<T, Hash> from JSON
     *
     * @param jsonData
     * @param set
     */
    static void from_json(const nlohmann::json& jsonData, Set<T, Hash>& set)
    {
        for (const auto& item : jsonData)
        {
            set.insert(item.get<T>());
        }
    }
};

/**
 * @brief Serializer for ShortcutSet
 *
 */
template <>
struct adl_serializer<ShortcutSet>
{
    /**
     * @brief Serialize ShortcutSet to JSON
     *
     * @param jsonData
     * @param shortcutSet
     */
    static void to_json(
        nlohmann::json&    jsonData,
        const ShortcutSet& shortcutSet
    )
    {
        jsonData["forWhat"]   = shortcutSet.getWhat();
        jsonData["context"]   = shortcutSet.getContext();
        jsonData["shortcuts"] = shortcutSet.getShortcuts();
    }

    /**
     * @brief Deserialize ShortcutSet from JSON
     *
     * @param jsonData
     * @return ShortcutSet
     */
    static ShortcutSet from_json(const nlohmann::json& jsonData)
    {
        const auto forWhat = jsonData.at("forWhat").get<std::string>();
        const auto context = jsonData.at("context").get<ShortcutContext>();
        const auto shortcuts =
            jsonData.at("shortcuts").get<nlohmann::json::array_t>();

        Set<Shortcut, Shortcut::Hash> shortcutSet;
        for (const auto& item : shortcuts)
        {
            shortcutSet.insert(item.get<Shortcut>());
        }

        return {forWhat, context, shortcutSet};
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_JSON_HPP__