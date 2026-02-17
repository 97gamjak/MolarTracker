#ifndef __SETTINGS__PARAM_CORE_TPP__
#define __SETTINGS__PARAM_CORE_TPP__

#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <string>

#include "param_core.hpp"
#include "param_error.hpp"

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
     * @brief Construct a new Param Core:: Param Core object
     *
     * @tparam T
     * @param key
     * @param title
     * @param description
     */
    template <typename T>
    ParamCore<T>::ParamCore(
        const std::string& key,
        const std::string& title,
        const std::string& description
    )
        : _key(key), _title(title), _description(description)
    {
    }

    /**
     * @brief Get the value of the parameter
     *
     * @tparam T
     * @return const T&
     */
    template <typename T>
    const std::optional<T>& ParamCore<T>::get() const
    {
        if (_value.has_value())
            return _value;

        return _defaultValue;
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
        const std::optional<T>& current = get();
        if (current.has_value() && _equals(current.value(), value))
            return;

        _value = value;
        _notifySubscribers();
    }

    /**
     * @brief Unset the value of the parameter, this will make get() return the
     * default value if it exists
     *
     * @tparam T
     */
    template <typename T>
    void ParamCore<T>::unset()
    {
        _value.reset();
        _notifySubscribers();
    }

    /**
     * @brief Subscribe to changes in the parameter value, the provided callback
     * function will be called whenever the value is changed or unset, the user
     * pointer can be used to pass additional data to the callback function, the
     * returned Connection object can be used to unsubscribe from changes
     *
     * @tparam T
     * @param fn The callback function to call when the parameter value changes,
     * it should have the signature void(void* user, const std::optional<T>&
     * newValue)
     * @param user A user-defined pointer that will be passed to the callback
     * function when it is called, this can be used to provide additional
     * context for the callback function
     * @return Connection An object representing the subscription, this can be
     * used to unsubscribe from changes by calling disconnect() on it or by
     * letting it go out of scope
     */
    template <typename T>
    Connection ParamCore<T>::subscribe(ChangedFn fn, void* user)
    {
        const auto id = _idCounter++;

        if (!fn)
            throw std::invalid_argument("Callback function cannot be null");

        if (_idCounter == 0)
        {
            // Handle the case where the id counter overflows and wraps around
            // to 0 This is a very unlikely scenario, but we should still handle
            // it to prevent potential issues with subscriber management
            throw ParamException("Subscriber ID counter has overflowed");
        }

        _subscribers[id] = Subscriber{fn, user};

        return Connection::make(this, id, &ParamCore<T>::_disconnect);
    }

    /**
     * @brief Commit the current value as the baseline for dirty checking
     *
     * @tparam T
     */
    template <typename T>
    void ParamCore<T>::commit()
    {
        _baseLine = _value;
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
        if (!_baseLine.has_value() && !_value.has_value())
            return false;

        if (_value.has_value() && !_baseLine.has_value())
            return true;

        if (_baseLine.has_value() && !_value.has_value())
            return true;

        return !_equals(_value.value(), _baseLine.value());
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
     * @brief Set whether changing this parameter requires a reboot, this can be
     * used to inform the user that they need to restart the application for
     * changes to take effect
     *
     * @tparam T
     * @param required
     */
    template <typename T>
    void ParamCore<T>::setRebootRequired(bool required)
    {
        _isRebootRequired = required;
    }

    /**
     * @brief Check if changing this parameter requires a reboot
     *
     * @tparam T
     * @return true if a reboot is required for changes to take effect, false
     * otherwise
     */
    template <typename T>
    bool ParamCore<T>::isRebootRequired() const
    {
        return _isRebootRequired;
    }

    /**
     * @brief Serialize ParamCore to JSON
     *
     * @tparam T
     * @return nlohmann::json
     */
    template <typename T>
    nlohmann::json ParamCore<T>::toJson() const
    {
        nlohmann::json jsonData;

        jsonData[Schema::KEY_KEY]         = _key;
        jsonData[Schema::TITLE_KEY]       = _title;
        jsonData[Schema::DESCRIPTION_KEY] = _description;
        jsonData[Schema::VALUE_KEY]       = _value;
        jsonData[Schema::DEFAULT_KEY]     = _defaultValue;

        return jsonData;
    }

    /**
     * @brief Deserialize ParamCore from JSON
     *
     * @tparam T
     * @param jsonData
     * @param param
     */
    template <typename T>
    void ParamCore<T>::fromJson(
        const nlohmann::json& jsonData,
        ParamCore<T>&         param
    )
    {
        param._key   = jsonData.at(Schema::KEY_KEY).get<std::string>();
        param._title = jsonData.at(Schema::TITLE_KEY).get<std::string>();
        param._description =
            jsonData.at(Schema::DESCRIPTION_KEY).get<std::string>();

        param._value = jsonData.at(Schema::VALUE_KEY).get<std::optional<T>>();
        param._defaultValue =
            jsonData.at(Schema::DEFAULT_KEY).get<std::optional<T>>();
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

    /**
     * @brief Disconnect a subscriber from the parameter, this is called by the
     * Connection object when it is destroyed or when disconnect() is called on
     * it
     *
     * @tparam T
     * @param owner
     * @param id
     */
    template <typename T>
    void ParamCore<T>::_disconnect(void* owner, std::size_t id)
    {
        auto* self = static_cast<ParamCore<T>*>(owner);
        self->_subscribers.erase(id);
    }

    /**
     * @brief Notify all subscribers of a change in the parameter value, this is
     * called whenever the value is changed or unset
     *
     * @tparam T
     */
    template <typename T>
    void ParamCore<T>::_notifySubscribers()
    {
        // Note: we use here a copy of the subscriber map to avoid issues when a
        // subscriber is added or removed while we are notifying subscribers,
        // this way we can safely iterate over the copy without worrying about
        // concurrent modifications to the original map
        std::unordered_map<size_t, Subscriber> copy = _subscribers;

        for (auto& [_, sub] : copy)
            sub.fn(sub.user, _value);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAM_CORE_TPP__