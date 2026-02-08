#ifndef __SETTINGS__PARAMS_CORE_TPP__
#define __SETTINGS__PARAMS_CORE_TPP__

#include "params_core.hpp"

namespace settings
{
    /**
     * @brief Construct a new Param Core:: Param Core object
     *
     * @tparam T
     * @param key
     * @param title
     */
    template <typename T>
    ParamCore<T>::ParamCore(const std::string& key, const std::string& title)
        : _key(key), _title(title)
    {
    }

    /**
     * @brief Get the value of the parameter
     *
     * @tparam T
     * @return const T&
     */
    template <typename T>
    const T& ParamCore<T>::get() const
    {
        return _value;
    }

    /**
     * @brief Set the value of the parameter
     *
     * @tparam T
     * @param value
     */
    template <typename T>
    void ParamCore<T>::set(const T& value)
    {
        _value = value;
    }

    /**
     * @brief Get the default value of the parameter
     *
     * @tparam T
     * @return const std::optional<T>&
     */
    template <typename T>
    const std::optional<T>& ParamCore<T>::getDefault() const
    {
        return _defaultValue;
    }

    /**
     * @brief Set the default value of the parameter
     *
     * @tparam T
     * @param defaultValue
     */
    template <typename T>
    void ParamCore<T>::setDefault(const std::optional<T>& defaultValue)
    {
        _defaultValue = defaultValue;
    }

    /**
     * @brief Get the key of the parameter
     *
     * @tparam T
     * @return const std::string&
     */
    template <typename T>
    const std::string& ParamCore<T>::getKey() const
    {
        return _key;
    }

    /**
     * @brief Get the title of the parameter
     *
     * @tparam T
     * @return const std::string&
     */
    template <typename T>
    const std::string& ParamCore<T>::getTitle() const
    {
        return _title;
    }

    /**
     * @brief Get the description of the parameter
     *
     * @tparam T
     * @return const std::string&
     */
    template <typename T>
    const std::string& ParamCore<T>::getDescription() const
    {
        return _description;
    }

    /**
     * @brief Set the description of the parameter
     *
     * @tparam T
     * @param description
     */
    template <typename T>
    void ParamCore<T>::setDescription(const std::string& description)
    {
        _description = description;
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS_CORE_TPP__