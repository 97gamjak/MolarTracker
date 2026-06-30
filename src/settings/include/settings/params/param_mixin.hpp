#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_MIXIN_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_MIXIN_HPP__

#include <optional>
#include <string>

#include "config/signal_tags.hpp"
#include "connections/connection.hpp"
#include "settings/params/param_core.hpp"

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
       public:
        /// alias for the type of the parameter value
        using value_type = T;

       public:
        [[nodiscard]] const std::optional<T>& getOptional() const;
        [[nodiscard]] const T&                get() const;

        [[nodiscard]] const std::optional<T>& getDefault() const;
        void setDefault(const std::optional<T>& defaultValue);

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] const std::string& getDescription() const;
        void setDescription(const std::string& description);

        [[nodiscard]] Connection subscribe(
            ParamValueChanged<T>::func func,
            void*                      user
        );
        [[nodiscard]] Connection subscribeToOptional(
            ParamOptionalChanged<T>::func func,
            void*                         user
        );
        [[nodiscard]] Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        [[nodiscard]] bool isRebootRequired() const;
        void               setRebootRequired(bool required);

        void commit();

        [[nodiscard]] bool isDirty() const;

       private:
        [[nodiscard]] Derived&       _self();
        [[nodiscard]] const Derived& _self() const;
    };

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_MIXIN_TPP__
#include "param_mixin.tpp"
#endif

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_MIXIN_HPP__