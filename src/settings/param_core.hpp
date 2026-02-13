#ifndef __SETTINGS__PARAM_CORE_HPP__
#define __SETTINGS__PARAM_CORE_HPP__

#include <optional>
#include <string>

#include "config/json.hpp"   // IWYU pragma: keep for adl_serializer of std::optional<T>

namespace settings
{

    class ParamCoreSchema
    {
       public:
        static constexpr const char* KEY_KEY         = "key";
        static constexpr const char* TITLE_KEY       = "title";
        static constexpr const char* DESCRIPTION_KEY = "description";
        static constexpr const char* VALUE_KEY       = "value";
        static constexpr const char* DEFAULT_KEY     = "default";
    };

    /**
     * @brief Core class for a setting parameter, this is a template class that
     * can be used for any type of setting parameter
     *
     * @tparam T
     */
    template <typename T>
    class ParamCore
    {
       private:
        using Schema = ParamCoreSchema;

        T    _value{};
        T    _baseLine{};
        bool _hasBaseLine = false;

        std::optional<T> _defaultValue = std::nullopt;

        std::string _key;
        std::string _title;
        std::string _description;

       public:
        ParamCore() = delete;
        ParamCore(const std::string& key, const std::string& title);

        [[nodiscard]] const T& get() const;
        void                   set(const T& value);

        void commit();

        [[nodiscard]] bool isDirty() const;

        [[nodiscard]] const std::optional<T>& getDefault() const;
        void setDefault(const std::optional<T>& defaultValue);

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] const std::string& getDescription() const;
        void setDescription(const std::string& description);

        nlohmann::json      toJson() const;
        static ParamCore<T> fromJson(const nlohmann::json& j);

       private:
        static bool _equals(const T& a, const T& b);
    };

}   // namespace settings

NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for utils::SemVer
 *
 */
template <typename T>
struct adl_serializer<settings::ParamCore<T>>
{
    /**
     * @brief Serialize ParamCore<T> to JSON
     *
     * @param j
     * @param settings
     */
    static void to_json(
        nlohmann::json&               j,
        const settings::ParamCore<T>& settings
    )
    {
        j = settings.toJson();
    }

    /**
     * @brief Deserialize ParamCore<T> from JSON
     *
     * @param j
     * @return settings::ParamCore<T>
     */
    static settings::ParamCore<T> from_json(const nlohmann::json& j)
    {
        return settings::ParamCore<T>::fromJson(j);
    }
};
NLOHMANN_JSON_NAMESPACE_END

#ifndef __SETTINGS__PARAM_CORE_TPP__
#include "param_core.tpp"
#endif   // __SETTINGS__PARAM_CORE_TPP__

#endif   // __SETTINGS__PARAM_CORE_HPP__