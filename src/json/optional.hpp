#ifndef __JSON__OPTIONAL_HPP__
#define __JSON__OPTIONAL_HPP__

#include <nlohmann/json.hpp>
#include <optional>

NLOHMANN_JSON_NAMESPACE_BEGIN

/**
 * @brief Serializer for std::optional<T>
 *
 * @tparam T
 */
template <typename T>
struct adl_serializer<std::optional<T>>
{
    /**
     * @brief Serialize std::optional<T> to JSON
     *
     * @param jsonData
     * @param opt
     */
    static void to_json(json& jsonData, const std::optional<T>& opt)
    {
        if (opt.has_value())
            jsonData = *opt;
        else
            jsonData = nullptr;
    }

    /**
     * @brief Deserialize std::optional<T> from JSON
     *
     * @param jsonData
     * @param opt
     */
    static void from_json(const json& jsonData, std::optional<T>& opt)
    {
        if (jsonData.is_null())
            opt = std::nullopt;
        else
            opt = jsonData.get<T>();
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __JSON__OPTIONAL_HPP__