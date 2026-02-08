#ifndef __SETTINGS__PARAMS_MIXIN_TPP__
#define __SETTINGS__PARAMS_MIXIN_TPP__

#include "params_mixin.hpp"

namespace settings
{
    /**
     * @brief Get the value of the parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const T&
     */
    template <typename Derived, typename T>
    const T& ParamMixin<Derived, T>::get() const
    {
        return _core().get();
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
        return _core().getDefault();
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
        _core().setDefault(defaultValue);
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
        return _core().getKey();
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
        return _core().getTitle();
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
        return _core().getDescription();
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
        _core().setDescription(description);
    }

    /**
     * @brief Get the core object
     *
     * @tparam Derived
     * @tparam T
     * @return ParamCore<T>&
     */
    template <typename Derived, typename T>
    ParamCore<T>& ParamMixin<Derived, T>::_core()
    {
        return _self()._core;
    }

    /**
     * @brief Get the core object (const)
     *
     * @tparam Derived
     * @tparam T
     * @return const ParamCore<T>&
     */
    template <typename Derived, typename T>
    const ParamCore<T>& ParamMixin<Derived, T>::_core() const
    {
        return _self()._core;
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

#endif   // __SETTINGS__PARAMS_MIXIN_TPP__