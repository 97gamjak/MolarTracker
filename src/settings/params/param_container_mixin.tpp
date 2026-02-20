#ifndef __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__
#define __SETTINGS__PARAMS__PARAM_CONTAINER_MIXIN_TPP__

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