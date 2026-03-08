#ifndef __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_HPP__
#define __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_HPP__

#include <string>
#include <vector>

#include "config/signal_tags.hpp"
#include "param_utils.hpp"

class Connection;   // Forward declaration

namespace settings
{
    /**
     * @brief Mixin class for parameter groups
     *
     * A parameter group aggregates multiple parameters into a single logical
     * unit (e.g. two NumericParams forming a 2D coordinate). Unlike
     * ParamContainerMixin — which is intended for top-level settings sections —
     * a group is designed to be embedded alongside regular params inside a
     * container's _forEachParam. It exposes isDirty() so callers can poll the
     * combined dirty state without subscribing.
     *
     * Requirements on the Derived class:
     *   - A `_core` member of type ParamContainer
     *   - `friend ParamGroupMixin<Derived>;`
     *   - `template<typename Func> void _forEachParam(Func&&) const;`
     *   - `template<typename Func> void _forEachParam(Func&&);`
     *
     * @tparam Derived The derived class that inherits from this mixin
     */
    template <typename Derived>
    class ParamGroupMixin
    {
       public:
        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const std::string& getDescription() const;

        /// Returns true if any sub-parameter differs from its baseline value
        [[nodiscard]] bool isDirty() const;

        [[nodiscard]] std::vector<Connection> subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        [[nodiscard]] nlohmann::json toJson() const;
        static void fromJson(const nlohmann::json& jsonData, Derived& group);

        void commit();

       private:
        [[nodiscard]] Derived&       _self();
        [[nodiscard]] const Derived& _self() const;
    };

}   // namespace settings

#ifndef __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_TPP__
#include "param_group_mixin.tpp"
#endif   // __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_TPP__

#endif   // __SETTINGS__PARAMS__PARAM_GROUP_MIXIN_HPP__
