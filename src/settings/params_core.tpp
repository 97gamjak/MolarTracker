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
     * @brief Commit the current value as the baseline for dirty checking
     *
     * @tparam T
     */
    template <typename T>
    void ParamCore<T>::commit()
    {
        _baseLine    = _value;
        _hasBaseLine = true;
    }

    /**
     * @brief Check if the current value is different from the baseline value
     *
     * @tparam T
     * @return true if the value has been changed since the last commit, false
     * otherwise
     */
    template <typename T>
    bool ParamCore<T>::isDirty() const
    {
        if (!_hasBaseLine)
            return false;

        return !_equals(_value, _baseLine);
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

    /**
     * @brief Check if two values are equal, this is used for dirty checking
     *
     * @tparam T
     * @param a
     * @param b
     * @return true if the values are equal, false otherwise
     */
    template <typename T>
    bool ParamCore<T>::_equals(const T& a, const T& b)
    {
        if constexpr (std::floating_point<T>)
        {
            // For floating-point types, consider them equal if they are within
            // a small epsilon value of each other to account for precision
            // issues
            const auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(a - b) <= epsilon;
        }
        else
        {
            // For non-floating-point types, use regular equality
            return a == b;
        }
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS_CORE_TPP__