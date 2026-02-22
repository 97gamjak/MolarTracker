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
     * @param j
     * @param opt
     */
    static void to_json(json& j, const std::optional<T>& opt)
    {
        if (opt.has_value())
            j = *opt;
        else
            j = nullptr;
    }

    /**
     * @brief Deserialize std::optional<T> from JSON
     *
     * @param j
     * @param opt
     */
    static void from_json(const json& j, std::optional<T>& opt)
    {
        if (j.is_null())
            opt = std::nullopt;
        else
            opt = j.get<T>();
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __JSON__OPTIONAL_HPP__