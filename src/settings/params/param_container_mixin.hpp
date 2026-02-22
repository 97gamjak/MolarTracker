#ifndef __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_HPP__
#define __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_HPP__

#include <string>
#include <vector>

#include "config/signal_tags.hpp"
#include "param_utils.hpp"

class Connection;   // Forward declaration

namespace settings
{
    /**
     * @brief Mixin class for parameter containers, this class provides common
     * functionality for all parameter containers, such as getting the key,
     * title, and description of the container. It is intended to be used as a
     * base class for specific parameter container classes like GeneralSettings
     * and UISettings.
     *
     * @tparam Derived The derived class that inherits from this mixin
     */
    template <typename Derived>
    class ParamContainerMixin
    {
       public:
        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const std::string& getDescription() const;

        [[nodiscard]] std::vector<Connection> subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        [[nodiscard]] nlohmann::json toJson() const;
        static void fromJson(const nlohmann::json& jsonData, Derived& settings);

       private:
        [[nodiscard]] Derived&       _self();
        [[nodiscard]] const Derived& _self() const;
    };

}   // namespace settings

#ifndef __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__
#include "param_container_mixin.tpp"
#endif   // __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__

#endif   // __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_HPP__