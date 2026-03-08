#ifndef __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_TPP__
#define __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_TPP__

#include "connections/connection.hpp"
#include "param_group_mixin.hpp"

namespace settings
{
    /**
     * @brief Get the key of the parameter group
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamGroupMixin<Derived>::getKey() const
    {
        return _self()._core.getKey();
    }

    /**
     * @brief Get the title of the parameter group
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamGroupMixin<Derived>::getTitle() const
    {
        return _self()._core.getTitle();
    }

    /**
     * @brief Get the description of the parameter group
     *
     * @tparam Derived
     * @return const std::string&
     */
    template <typename Derived>
    const std::string& ParamGroupMixin<Derived>::getDescription() const
    {
        return _self()._core.getDescription();
    }

    /**
     * @brief Check whether any sub-parameter differs from its baseline value
     *
     * Iterates all sub-parameters via _forEachParam and returns true as soon
     * as any one of them reports isDirty() == true.
     *
     * @tparam Derived
     * @return true if at least one sub-parameter is dirty, false otherwise
     */
    template <typename Derived>
    bool ParamGroupMixin<Derived>::isDirty() const
    {
        bool dirty = false;

        auto checkDirty = [&](const auto& param) { dirty = dirty || param.isDirty(); };

        _self()._forEachParam(checkDirty);

        return dirty;
    }

    /**
     * @brief Subscribe to changes in the dirty state of any sub-parameter
     *
     * The callback is called whenever the dirty state of any sub-parameter
     * changes. Both Connection (from single params) and std::vector<Connection>
     * (from nested groups) are handled transparently.
     *
     * @tparam Derived
     * @param func Callback with signature void(void* user, bool isDirty)
     * @param user User-defined pointer passed to the callback
     * @return std::vector<Connection> One connection per sub-parameter
     */
    template <typename Derived>
    std::vector<Connection> ParamGroupMixin<Derived>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        std::vector<Connection> connections;

        auto subscribeToDirtyForParam = [&](auto& param)
        {
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
     * @brief Serialize the parameter group to a JSON object
     *
     * Each sub-parameter is serialized under its own key, producing a nested
     * JSON object (e.g. {"x": 1.0, "y": 2.0}).
     *
     * @tparam Derived
     * @return nlohmann::json
     */
    template <typename Derived>
    nlohmann::json ParamGroupMixin<Derived>::toJson() const
    {
        nlohmann::json jsonData;

        auto paramToJson = [&](const auto& param)
        { jsonData[param.getKey()] = param.toJson(); };

        _self()._forEachParam(paramToJson);

        return jsonData;
    }

    /**
     * @brief Deserialize the parameter group from a JSON object
     *
     * @tparam Derived
     * @param jsonData JSON object matching the structure produced by toJson()
     * @param group    The group instance to deserialize into
     */
    template <typename Derived>
    void ParamGroupMixin<Derived>::fromJson(
        const nlohmann::json& jsonData,
        Derived&              group
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

        group._forEachParam(paramFromJson);
    }

    /**
     * @brief Commit all sub-parameters, clearing their dirty state
     *
     * @tparam Derived
     */
    template <typename Derived>
    void ParamGroupMixin<Derived>::commit()
    {
        auto commitParam = [&](auto& param) { param.commit(); };

        _self()._forEachParam(commitParam);
    }

    /**
     * @brief Get a reference to the derived class (CRTP helper)
     *
     * @tparam Derived
     * @return Derived&
     */
    template <typename Derived>
    Derived& ParamGroupMixin<Derived>::_self()
    {
        return static_cast<Derived&>(*this);
    }

    /**
     * @brief Get a const reference to the derived class (CRTP helper)
     *
     * @tparam Derived
     * @return const Derived&
     */
    template <typename Derived>
    const Derived& ParamGroupMixin<Derived>::_self() const
    {
        return static_cast<const Derived&>(*this);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_TPP__
