#ifndef __SETTINGS__PARAM_MIXIN_HPP__
#define __SETTINGS__PARAM_MIXIN_HPP__

#include <optional>
#include <string>

#include "connections/connection.hpp"

namespace settings
{

    /**
     * @brief Mixin class for a setting parameter, this is a template class that
     * can be used for any type of setting parameter, this is used to add
     * additional functionality to the ParamCore class
     *
     * @tparam T
     */
    template <typename Derived, typename T>
    class ParamMixin
    {
       private:
        template <typename U>
        using ChangedFnBase     = void (*)(void*, const U&);
        using ChangedFn         = ChangedFnBase<T>;
        using ChangedFnOptional = ChangedFnBase<std::optional<T>>;

       public:
        [[nodiscard]] const std::optional<T>& getOptional() const;
        [[nodiscard]] const T&                get() const;

        [[nodiscard]] const std::optional<T>& getDefault() const;
        void setDefault(const std::optional<T>& defaultValue);

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] const std::string& getDescription() const;
        void setDescription(const std::string& description);

        Connection subscribe(ChangedFn func, void* user);
        Connection subscribeToOptional(ChangedFnOptional func, void* user);

        [[nodiscard]] bool isRebootRequired() const;
        void               setRebootRequired(bool required);

       private:
        [[nodiscard]] Derived&       _self();
        [[nodiscard]] const Derived& _self() const;
    };

}   // namespace settings

#ifndef __SETTINGS__PARAM_MIXIN_TPP__
#include "param_mixin.tpp"
#endif   // __SETTINGS__PARAM_MIXIN_TPP__

#endif   // __SETTINGS__PARAM_MIXIN_HPP__