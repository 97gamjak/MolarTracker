#ifndef __SETTINGS__PARAMS__PARAM_MIXIN_HPP__
#define __SETTINGS__PARAMS__PARAM_MIXIN_HPP__

#include <optional>
#include <string>

#include "config/signal_tags.hpp"
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
        /// Type alias for the templated change callback function, this is a
        /// function pointer that takes a pointer to the user data and the new
        /// value of the parameter, this is used to notify subscribers when the
        /// parameter value changes
        template <typename U>
        using ChangedFnBase = void (*)(void*, const U&);

        /// Type alias for the change callback function for the parameter value,
        /// this is a function pointer that takes a pointer to the user data and
        /// the new value of the parameter, this is used to notify subscribers
        /// when the parameter value changes
        using ChangedFn = ChangedFnBase<T>;

        /// Type alias for the change callback function for the optional
        /// parameter value, this is a function pointer that takes a pointer to
        /// the user data and the new optional value of the parameter, this is
        /// used to notify subscribers when the parameter value changes, this is
        /// useful for parameters that can be unset (optional)
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

        [[nodiscard]] Connection subscribe(ChangedFn func, void* user);
        [[nodiscard]] Connection subscribeToOptional(
            ChangedFnOptional func,
            void*             user
        );
        [[nodiscard]] Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

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

#endif   // __SETTINGS__PARAMS__PARAM_MIXIN_HPP__