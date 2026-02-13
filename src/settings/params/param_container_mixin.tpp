#ifndef __SETTINGS__PARAM_CONTAINER_MIXIN_TPP__
#define __SETTINGS__PARAM_CONTAINER_MIXIN_TPP__

#include "param_container_mixin.hpp"

namespace settings
{
    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getKey() const
    {
        return _self().core().getKey();
    }

    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getTitle() const
    {
        return _self().core().getTitle();
    }

    template <typename Derived>
    const std::string& ParamContainerMixin<Derived>::getDescription() const
    {
        return _self().core().getDescription();
    }

    template <typename Derived>
    Derived& ParamContainerMixin<Derived>::_self()
    {
        return static_cast<Derived&>(*this);
    }

    template <typename Derived>
    const Derived& ParamContainerMixin<Derived>::_self() const
    {
        return static_cast<const Derived&>(*this);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAM_CONTAINER_MIXIN_TPP__