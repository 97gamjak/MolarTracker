#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_HPP__

#include "error/param_error.hpp"
#include "param_core.hpp"
#include "settings/params/i_param.hpp"

namespace settings
{

    /**
     * @brief A map of string keys to ParamCore values. This class provides
     *        methods to manage the map, including adding new parameters,
     *        validating them, and serializing/deserializing to/from JSON.
     *
     * @tparam Value The type of the values in the map.
     */
    template <typename Value>
    class ParamMap : public Map<std::string, ParamCore<Value>>
    {
       private:
        /// Using Base to refer to the base class for convenience
        using Base = Map<std::string, ParamCore<Value>>;

       public:
        using Base::Base;
    };

    /**
     * @brief Schema for JSON serialization/deserialization of MapParam.
     */
    struct MapParamSchema
    {
        /// key for the parameter key in JSON
        static constexpr const char* KEY_KEY = "key";

        /// key for the parameter title in JSON
        static constexpr const char* TITLE_KEY = "title";

        /// key for the parameter description in JSON
        static constexpr const char* DESCRIPTION_KEY = "description";

        /// key for the parameter values in JSON
        static constexpr const char* PARAMS_MAP_KEY = "params";
    };

    /**
     * @brief Represents a parameter that is a map of string keys to ParamCore
     *        values. This class provides methods to manage the map, including
     *        adding new parameters, validating them, and
     * serializing/deserializing to/from JSON.
     *
     * @tparam Value The type of the values in the map.
     */
    template <typename Value>
    class MapParam : public Observable<OnDirtyChanged>, public IParam
    {
       private:
        /// Key for the parameter
        std::string _key;

        /// Title for the parameter
        std::string _title;

        /// Description for the parameter
        std::string _description;

        /// Vector of numeric parameters
        ParamMap<Value> _params;

        /// Flag indicating whether the parameter has been modified since the
        /// last commit or reset operation.
        using ValidatorFunc = std::function<
            ParamResult<void>(const ParamCore<Value>&, const ParamMap<Value>&)>;

        /// Vector of validator functions to validate the parameters in the map.
        std::vector<ValidatorFunc> _validators;

        /// Schema for JSON serialization/deserialization
        using Schema = MapParamSchema;

       public:
        MapParam(std::string key, std::string title, std::string description);

        [[nodiscard]] const std::string& getKey() const override;
        [[nodiscard]] const std::string& getTitle() const override;
        [[nodiscard]] const std::string& getDescription() const override;
        [[nodiscard]] std::string        toString() const override;

        bool isDirty() const override;
        void commit() override;
        void resetToDefault() override;

        ParamResult<void> addParam(const ParamCore<Value>& value);
        void              addValidator(ValidatorFunc validator);

        static void fromJson(
            const nlohmann::json& jsonData,
            MapParam<Value>&      param
        );
        [[nodiscard]] nlohmann::json toJson() const override;

        [[nodiscard]]
        Connections subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) override;

        [[nodiscard]] const ParamMap<Value>& getParams() const;
        [[nodiscard]]
        ParamResult<void> setAt(const std::string& key, const Value& value);
        [[nodiscard]] const ParamCore<Value>& at(const std::string& key) const;
    };

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_TPP__
#include "map_param.tpp"
#endif   //  __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_TPP__

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_HPP__