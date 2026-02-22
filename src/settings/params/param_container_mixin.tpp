#ifndef __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__
#define __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__

#include "connections/connection.hpp"
#include "param_container_mixin.hpp"

namespace settings
{
    /**
     * @brief Get the key of the parameter container
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getKey() const
    {
        return _self()._core.getKey();
    }

    /**
     * @brief Get the title of the parameter container
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getTitle() const
    {
        return _self()._core.getTitle();
    }

    /**
     * @brief Get the description of the parameter container
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getDescription() const
    {
        return _self()._core.getDescription();
    }

    /**
     * @brief Subscribe to changes in the dirty state of any parameter in the
     * container, the provided callback function will be called whenever the
     * dirty state of any parameter changes, the user pointer can be used to
     * pass additional data to the callback function, the returned Connection
     * objects can be used to unsubscribe from changes
     *
     * @param func The callback function to call when the dirty state of any
     * parameter changes, it should have the signature void(void* user, bool
     * isDirty)
     * @param user A user-defined pointer that will be passed to the callback
     * function when it is called, this can be used to provide additional
     * context for the callback function
     * @return std::vector<Connection> A vector of Connection objects
     * representing the subscriptions, these can be used to unsubscribe from
     * changes by calling disconnect() on them or by letting them go out of
     * scope
     */
    template <typename Derived>
    std::vector<Connection> ParamContainerMixin<Derived>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        std::vector<Connection> connections;

        auto subscribeToDirtyForParam = [&](auto& param)
        {
            // subscribe to dirty can either return a single Connection or a
            // vector of Connections, we need to handle both cases
            if constexpr (std::is_same_v<
                              decltype(param.subscribeToDirty(func, user)),
                              Connection>)
            {
                auto connection = param.subscribeToDirty(func, user);
                connections.push_back(std::move(connection));
            }
            else
            {
                auto paramConnections = param.subscribeToDirty(func, user);
                for (auto& connection : paramConnections)
                    connections.push_back(std::move(connection));
            }
        };

        _self()._forEachParam(subscribeToDirtyForParam);

        return connections;
    }

    /**
     * @brief Serialize the parameter container to JSON, this will serialize all
     * parameters in the container to a JSON object, the exact structure of the
     * JSON will depend on the implementation of the parameters and how they
     * serialize themselves, but it will generally be a JSON object with keys
     * corresponding to the parameter keys and values corresponding to the
     * parameter values
     *
     * @tparam Derived
     * @return nlohmann::json A JSON object representing the serialized
     * parameter container
     */
    template <typename Derived>
    nlohmann::json ParamContainerMixin<Derived>::toJson() const
    {
        nlohmann::json jsonData;

        auto paramToJson = [&](const auto& param)
        { jsonData[param.getKey()] = param.toJson(); };

        _self()._forEachParam(paramToJson);

        return jsonData;
    }

    /**
     * @brief Deserialize the parameter container from JSON, this will
     * deserialize all parameters in the container from a JSON object, the exact
     * structure of the JSON should match the structure produced by toJson(),
     * but it will generally be a JSON object with keys corresponding to the
     * parameter keys and values corresponding to the parameter values, this
     * function will call the fromJson() function of each parameter to
     * deserialize it from the JSON
     *
     * @tparam Derived
     * @param jsonData A JSON object representing the serialized parameter
     * container, this should match the structure produced by toJson()
     * @param settings The parameter container to deserialize into, this will be
     * modified by this function to contain the deserialized parameters
     */
    template <typename Derived>
    void ParamContainerMixin<Derived>::fromJson(
        const nlohmann::json& jsonData,
        Derived&              settings
    )
    {
        auto paramFromJson = [&](auto& param)
        {
            const auto& key = param.getKey();
            if (jsonData.contains(key))
            {
                std::remove_cvref_t<decltype(param)>::fromJson(
                    jsonData[key],
                    param
                );
            }
        };

        settings._forEachParam(paramFromJson);
    }

    /**
     * @brief Get a reference to the derived class, this is used for CRTP to
     * call functions of the derived class from the mixin
     *
     * @tparam Derived
     * @return Derived&
     */
    template <typename Derived>
    Derived& ParamContainerMixin<Derived>::_self()
    {
        return static_cast<Derived&>(*this);
    }

    /**
     * @brief Get a const reference to the derived class, this is used for CRTP
     * to call functions of the derived class from the mixin
     *
     * @tparam Derived
     * @return const Derived&
     */
    template <typename Derived>
    const Derived& ParamContainerMixin<Derived>::_self() const
    {
        return static_cast<const Derived&>(*this);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__