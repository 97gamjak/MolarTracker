#ifndef __SETTINGS__PARAM_MIXIN_TPP__
#define __SETTINGS__PARAM_MIXIN_TPP__

#include "param_mixin.hpp"

namespace settings
{

    /**
     * @brief Get the value of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const std::optional<T>&
     */

    template <typename Derived, typename T>
    const std::optional<T>& ParamMixin<Derived, T>::getOptional() const
    {
        return _self()._core.getOptional();
    }

    /**
     * @brief Get the value of the parameter, if the value is not set, it will
     * return the default value if it exists, otherwise it will throw an
     * exception
     *
     * @throws ParamException if the value is not set and no default value is
     * provided
     *
     * @tparam Derived
     * @tparam T
     * @return const T&
     */
    template <typename Derived, typename T>
    const T& ParamMixin<Derived, T>::get() const
    {
        return _self()._core.get();
    }

    /**
     * @brief Get the default value of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const std::optional<T>&
     */
    template <typename Derived, typename T>
    const std::optional<T>& ParamMixin<Derived, T>::getDefault() const
    {
        return _self()._core.getDefault();
    }

    /**
     * @brief Set the default value of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @param defaultValue
     */
    template <typename Derived, typename T>
    void ParamMixin<Derived, T>::setDefault(
        const std::optional<T>& defaultValue
    )
    {
        _self()._core.setDefault(defaultValue);
    }

    /**
     * @brief Get the key of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const std::string&
     */
    template <typename Derived, typename T>
    const std::string& ParamMixin<Derived, T>::getKey() const
    {
        return _self()._core.getKey();
    }

    /**
     * @brief Get the title of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const std::string&
     */
    template <typename Derived, typename T>
    const std::string& ParamMixin<Derived, T>::getTitle() const
    {
        return _self()._core.getTitle();
    }

    /**
     * @brief Get the description of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const std::string&
     */
    template <typename Derived, typename T>
    const std::string& ParamMixin<Derived, T>::getDescription() const
    {
        return _self()._core.getDescription();
    }

    /**
     * @brief Set the description of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @param description
     */
    template <typename Derived, typename T>
    void ParamMixin<Derived, T>::setDescription(const std::string& description)
    {
        _self()._core.setDescription(description);
    }

    /**
     * @brief Subscribe to changes of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @param func
     * @param user
     * @return Connection
     */
    template <typename Derived, typename T>
    Connection ParamMixin<Derived, T>::subscribe(ChangedFn func, void* user)
    {
        return _self()._core.subscribe(func, user);
    }

    /**
     * @brief Subscribe to changes of the parameter with return type
     * std::optional<T>
     *
     * @tparam Derived
     * @tparam T
     * @param func
     * @param user
     * @return Connection
     */
    template <typename Derived, typename T>
    Connection ParamMixin<Derived, T>::subscribeToOptional(
        ChangedFnOptional func,
        void*             user
    )
    {
        return _self()._core.subscribeToOptional(func, user);
    }

    /**
     * @brief Check if changing this parameter requires a reboot
     *
     * @tparam Derived
     * @tparam T
     * @return true if a reboot is required for changes to take effect, false
     * otherwise
     */
    template <typename Derived, typename T>
    bool ParamMixin<Derived, T>::isRebootRequired() const
    {
        return _self()._core.isRebootRequired();
    }

    /**
     * @brief Set whether changing this parameter requires a reboot, this can be
     * used to inform the user that they need to restart the application for
     * changes to take effect
     *
     * @tparam Derived
     * @tparam T
     * @param required
     */
    template <typename Derived, typename T>
    void ParamMixin<Derived, T>::setRebootRequired(bool required)
    {
        _self()._core.setRebootRequired(required);
    }

    /**
     * @brief Get the derived object
     *
     * @tparam Derived
     * @tparam T
     * @return Derived&
     */
    template <typename Derived, typename T>
    Derived& ParamMixin<Derived, T>::_self()
    {
        return static_cast<Derived&>(*this);
    }

    /**
     * @brief Get the derived object (const)
     *
     * @tparam Derived
     * @tparam T
     * @return const Derived&
     */
    template <typename Derived, typename T>
    const Derived& ParamMixin<Derived, T>::_self() const
    {
        return static_cast<const Derived&>(*this);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAM_MIXIN_TPP__