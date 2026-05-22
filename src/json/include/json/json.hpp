#ifndef __JSON__INCLUDE__JSON__JSON_HPP__
#define __JSON__INCLUDE__JSON__JSON_HPP__

#include <nlohmann/json.hpp>

namespace json
{
    /**
     * @brief Safely get a string value from a JSON object.
     *
     * @tparam T
     * @param obj
     * @param key
     * @param fallback
     * @return T
     */
    template <typename T>
    T _safeGet(
        const nlohmann::json& obj,
        const char*           key,
        const T&              fallback = {}
    )
    {
        if (const auto it = obj.find(key); it != obj.end() && !it->is_null())
            return it->get<T>();
        return fallback;
    }
}   // namespace json

#endif   // __JSON__INCLUDE__JSON__JSON_HPP__