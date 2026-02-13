#ifndef __SETTINGS__PARAM_CONTAINER_MIXIN_HPP__
#define __SETTINGS__PARAM_CONTAINER_MIXIN_HPP__

#include <string>

namespace settings
{
    template <typename Derived>
    class ParamContainerMixin
    {
       public:
        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const std::string& getDescription() const;

       private:
        [[nodiscard]] Derived&       _self();
        [[nodiscard]] const Derived& _self() const;
    };

}   // namespace settings

#ifndef __SETTINGS__PARAM_CONTAINER_MIXIN_TPP__
#include "param_container_mixin.tpp"
#endif   // __SETTINGS__PARAM_CONTAINER_MIXIN_TPP__

#endif   // __SETTINGS__PARAM_CONTAINER_MIXIN_HPP__