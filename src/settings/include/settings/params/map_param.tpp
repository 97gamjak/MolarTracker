#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_TPP__

#include "common/shortcut_json.hpp"   // IWYU pragma: keep -- needed for shortcut param serialization
#include "error/param_error.hpp"
#include "map_param.hpp"

namespace settings
{
    /**
     * @brief Constructs a MapParam object with the given key, title, and
     * description.
     *
     * @param key The unique key for the parameter.
     * @param title The human-readable title for the parameter.
     * @param description A brief description of the parameter.
     */
    template <typename Value>
    MapParam<Value>::MapParam(
        std::string key,
        std::string title,
        std::string description
    )
        : _key(std::move(key)),
          _title(std::move(title)),
          _description(std::move(description))
    {
    }

    /**
     * @brief Gets the key of the parameter. This is used to identify the
     * parameter in a map or list of parameters, and it should be unique within
     * the context of the parameters it is used in.
     *
     * @return const std::string& The key of the parameter.
     */
    template <typename Value>
    const std::string& MapParam<Value>::getKey() const
    {
        return _key;
    }

    /**
     * @brief Returns the title of the parameter.
     *
     * @return const std::string& The title of the parameter.
     */
    template <typename Value>
    const std::string& MapParam<Value>::getTitle() const
    {
        return _title;
    }

    /**
     * @brief Returns the description of the parameter.
     *
     * @return const std::string& The description of the parameter.
     */
    template <typename Value>
    const std::string& MapParam<Value>::getDescription() const
    {
        return _description;
    }

    /**
     * @brief Returns a string representation of the MapParam object, including
     *        its key, title, description, and all contained parameters.
     *
     * @return std::string The string representation of the MapParam.
     */
    template <typename Value>
    std::string MapParam<Value>::toString() const
    {
        std::string result = "MapParam(";
        for (const auto& [key, param] : _params)
        {
            result += key + ": " + param.get().toString() + ", ";
        }
        result += ")";
        return result;
    }

    /**
     * @brief Returns a const reference to the internal map of parameters.
     *
     * @return const ParamMap<Value>& The internal map of parameters.
     */
    template <typename Value>
    bool MapParam<Value>::isDirty() const
    {
        for (const auto& param : _params)
        {
            if (param.second.isDirty())
                return true;
        }
        return false;
    }

    /**
     * @brief Commits the current values of all parameters in the map, marking
     *        them as not dirty.
     */
    template <typename Value>
    void MapParam<Value>::commit()
    {
        for (auto& [key, param] : _params)
            param.commit();
    }

    /**
     * @brief Resets all parameters in the map to their default values.
     */
    template <typename Value>
    void MapParam<Value>::resetToDefault()
    {
        for (auto& [key, param] : _params)
            param.resetToDefault();
    }

    /**
     * @brief Subscribes to dirty state changes for all parameters in the map.
     *        The provided callback function will be called whenever any
     *        parameter's dirty state changes.
     *
     * @param func The callback function to call on dirty state changes.
     * @param user A user-defined pointer that will be passed to the callback.
     * @return Connections A collection of connections to manage the
     * subscriptions.
     */
    template <typename Value>
    Connections MapParam<Value>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        Connections connections;
        for (auto& [key, param] : _params)
        {
            auto connection = param.subscribeToDirty(func, user);
            connections.add(std::move(connection));
        }

        return connections;
    }

    /**
     * @brief Serializes the MapParam object to a JSON representation.
     *
     * @return nlohmann::json The JSON representation of the MapParam.
     */
    template <typename Value>
    nlohmann::json MapParam<Value>::toJson() const
    {
        nlohmann::json jsonData;
        jsonData[Schema::KEY_KEY]         = _key;
        jsonData[Schema::TITLE_KEY]       = _title;
        jsonData[Schema::DESCRIPTION_KEY] = _description;

        nlohmann::json paramsJson = nlohmann::json::array();
        for (const auto& [key, param] : _params)
            paramsJson.push_back(param.toJson());

        jsonData[Schema::PARAMS_MAP_KEY] = paramsJson;

        return jsonData;
    }

    /**
     * @brief Deserializes a JSON representation into a MapParam object.
     *
     * @param jsonData The JSON data to deserialize.
     * @param param The MapParam object to populate with the deserialized data.
     */
    template <typename Value>
    void MapParam<Value>::fromJson(
        const nlohmann::json& jsonData,
        MapParam<Value>&      param
    )
    {
        if (jsonData.contains(Schema::KEY_KEY))
            param._key = jsonData[Schema::KEY_KEY].get<std::string>();

        if (jsonData.contains(Schema::TITLE_KEY))
            param._title = jsonData[Schema::TITLE_KEY].get<std::string>();

        if (jsonData.contains(Schema::DESCRIPTION_KEY))
            param._description =
                jsonData[Schema::DESCRIPTION_KEY].get<std::string>();

        if (jsonData.contains(Schema::PARAMS_MAP_KEY))
        {
            const auto& paramsJson = jsonData[Schema::PARAMS_MAP_KEY];
            for (const auto& paramJson : paramsJson)
            {
                auto key   = paramJson.at(Schema::KEY_KEY).get<std::string>();
                auto title = paramJson.at(Schema::TITLE_KEY).get<std::string>();
                auto description =
                    paramJson.at(Schema::DESCRIPTION_KEY).get<std::string>();

                ParamCore<Value> newParam{
                    key,
                    title,
                    description
                };   // real ctor, not default
                ParamCore<Value>::fromJson(
                    paramJson,
                    newParam
                );   // now just populates value/defaults/etc.

                param._params.addUnchecked(key, newParam);
            }
        }
    }

    /**
     * @brief Adds a validator function to the list of validators for the
     * MapParam. The validator will be called whenever a parameter is added or
     * updated, and it can enforce custom validation rules.
     *
     * @param validator The validator function to add.
     */
    template <typename Value>
    void MapParam<Value>::addValidator(ValidatorFunc validator)
    {
        _validators.push_back(std::move(validator));
    }

    /**
     * @brief Adds a new parameter to the map, bypassing validation. This is
     * useful for loading parameters from a saved state where validation has
     * already been performed.
     * @param value The ParamCore object to add to the map.
     *
     * @return ParamResult<void> Result of the operation, containing an error if
     *         the key already exists in the map.
     */
    template <typename Value>
    ParamResult<void> MapParam<Value>::addParam(const ParamCore<Value>& value)
    {
        for (const auto& validator : _validators)
        {
            auto result = validator(value, _params);
            if (!result)
                return result.error();
        }

        _params.addUnchecked(value.getKey(), value);

        return {};
    }

    /**
     * @brief Returns a const reference to the internal map of parameters.
     *
     * @return const ParamMap<Value>& The internal map of parameters.
     */
    template <typename Value>
    const ParamMap<Value>& MapParam<Value>::getParams() const
    {
        return _params;
    }

    /**
     * @brief Sets the value of a parameter at the specified key. The new value
     *        is validated against all registered validators before being set.
     *
     * @param key The key of the parameter to update.
     * @param value The new value to set for the parameter.
     * @return ParamResult<void> Result of the operation, containing an error if
     *         validation fails or if the key does not exist.
     */
    template <typename Value>
    ParamResult<void> MapParam<Value>::setAt(
        const std::string& key,
        const Value&       value
    )
    {
        if (!_params.contains(key))
        {
            return ParamError{
                ParamErrorType::ParamNotFound,
                "Key not found in MapParam: " + key
            };
        }

        for (const auto& validator : _validators)
        {
            auto param = _params.at(key);   // copy to validate
            param.set(value);               // temporarily set to validate
            auto result = validator(param, _params);
            if (!result)
                return result.error();
        }

        auto& param = _params.at(key);
        param.set(value);

        return {};
    }

    /**
     * @brief Returns a const reference to the parameter at the specified key.
     *
     * @param key The key of the parameter to retrieve.
     * @return const ParamCore<Value>& The parameter at the specified key.
     */
    template <typename Value>
    const ParamCore<Value>& MapParam<Value>::at(const std::string& key) const
    {
        return _params.at(key);
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__MAP_PARAM_TPP__