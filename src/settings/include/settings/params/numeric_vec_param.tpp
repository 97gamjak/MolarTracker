#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__

#include "numeric_vec_param.hpp"

namespace settings
{
    /**
     * @brief Construct a new Numeric Vec Param:: Numeric Vec Param object
     *
     * @tparam T
     * @tparam N
     * @param keyPrefix
     * @param titlePrefix
     * @param descriptionPrefix
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    NumericVecParam<T, N>::NumericVecParam(
        const std::string& keyPrefix,
        const std::string& titlePrefix,
        const std::string& descriptionPrefix
    )
        : _key(keyPrefix), _title(titlePrefix), _description(descriptionPrefix)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            auto titlePrefixCopy       = titlePrefix;
            auto descriptionPrefixCopy = descriptionPrefix;

            std::string indexSuffix = std::to_string(i);
            _params.emplace_back(
                keyPrefix + indexSuffix,
                titlePrefixCopy.append(" " + indexSuffix),
                descriptionPrefixCopy.append(" (index " + indexSuffix + ")")
            );
        }
    }

    /**
     * @brief Set the default values for the numeric vector parameter, this
     * function sets the default value for each individual numeric parameter in
     * the vector, the size of the provided defaultValues vector must match the
     * size of the numeric vector parameter (N), otherwise an exception is
     * thrown
     *
     * @tparam T
     * @tparam N
     * @param defaultValues A vector of default values to set for each numeric
     * parameter in the vector, the size of this vector must be equal to N
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::setDefaults(const std::vector<T>& defaultValues)
    {
        if (defaultValues.size() != N)
        {
            throw ParamException(
                "The size of the default values vector must be equal to " +
                std::to_string(N)
            );
        }

        for (std::size_t i = 0; i < N; ++i)
            _params[i].setDefault(defaultValues[i]);
    }

    /**
     * @brief Set the default values for the numeric vector parameter, this
     * function sets the default value for each individual numeric parameter in
     * the vector, the size of the provided defaultValues vector must match the
     * size of the numeric vector parameter (N), otherwise an exception is
     * thrown
     *
     * @tparam T
     * @tparam N
     * @param defaultValues A pair of default values to set for each numeric
     * parameter in the vector, this overload is only available when N == 2, it
     * provides a convenient way to set default values for a 2D vector parameter
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::setDefaults(
        const std::pair<T, T>& defaultValues
    )
    requires(N == 2)
    {
        setDefaults(std::vector<T>{defaultValues.first, defaultValues.second});
    }

    /**
     * @brief Set the minimum values for the numeric vector parameter, this
     * function sets the minimum value for each individual numeric parameter in
     * the vector, the size of the provided minValues vector must match the
     * size of the numeric vector parameter (N), otherwise an exception is
     * thrown
     *
     * @tparam T
     * @tparam N
     * @param minValues A vector of minimum values to set for each numeric
     * parameter in the vector, the size of this vector must be equal to N
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::setMinValues(const std::vector<T>& minValues)
    {
        if (minValues.size() != N)
        {
            throw ParamException(
                "The size of the minimum values vector must be equal to " +
                std::to_string(N)
            );
        }

        for (std::size_t i = 0; i < N; ++i)
            _params[i].setMinValue(minValues[i]);
    }

    /**
     * @brief Set the minimum values for the numeric vector parameter, this
     * function sets the minimum value for each individual numeric parameter in
     * the vector, the size of the provided minValues vector must match the
     * size of the numeric vector parameter (N), otherwise an exception is
     * thrown
     *
     * @tparam T
     * @tparam N
     * @param minValues A pair of minimum values to set for each numeric
     * parameter in the vector, this overload is only available when N == 2, it
     * provides a convenient way to set minimum values for a 2D vector parameter
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::setMinValues(const std::pair<T, T>& minValues)
    requires(N == 2)
    {
        setMinValues(std::vector<T>{minValues.first, minValues.second});
    }

    /**
     * @brief Commit the current values of the numeric vector parameter, this
     * function calls commit() on each individual numeric parameter in the
     * vector to save their current values as the baseline for dirty checking
     *
     * @tparam T
     * @tparam N
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::commit()
    {
        for (auto& param : _params)
            param.commit();
    }

    /**
     * @brief Deserialize a numeric vector parameter from JSON, this function
     * reads the key, title, description, and an array of values for each
     * individual numeric parameter in the vector from the provided JSON data,
     * the size of the array in the JSON data must match the size of the
     * numeric vector parameter (N), otherwise an exception is thrown
     *
     * @tparam T
     * @tparam N
     * @param jsonData The JSON data to deserialize from, this should contain
     * the key, title, description, and an array of values for each individual
     * numeric parameter in the vector
     * @param param The NumericVecParam object to populate with the deserialized
     * data from the JSON
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    void NumericVecParam<T, N>::fromJson(
        const nlohmann::json&  jsonData,
        NumericVecParam<T, N>& param
    )
    {
        param._key   = jsonData.at(Schema::KEY_KEY).get<std::string>();
        param._title = jsonData.at(Schema::TITLE_KEY).get<std::string>();
        param._description =
            jsonData.at(Schema::DESCRIPTION_KEY).get<std::string>();

        const auto& paramsJson = jsonData.at(Schema::PARAMS_VEC_KEY);
        if (!paramsJson.is_array())
        {
            throw ParamException(
                "Expected an array for the 'params' field in the JSON data"
            );
        }

        if (paramsJson.size() != N)
        {
            throw ParamException(
                "The size of the 'params' array in the JSON data must be equal "
                "to " +
                std::to_string(N)
            );
        }

        for (std::size_t i = 0; i < N; ++i)
            NumericParam<T>::fromJson(paramsJson.at(i), param._params[i]);
    }

    /**
     * @brief Serialize the numeric vector parameter to JSON, this function
     * creates a JSON object with the key, title, description, and an array of
     * values for each individual numeric parameter in the vector
     *
     * @tparam T
     * @tparam N
     * @return nlohmann::json A JSON object representing the numeric vector
     * parameter
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    nlohmann::json NumericVecParam<T, N>::toJson() const
    {
        nlohmann::json jsonData;
        jsonData[Schema::KEY_KEY]         = _key;
        jsonData[Schema::TITLE_KEY]       = _title;
        jsonData[Schema::DESCRIPTION_KEY] = _description;

        nlohmann::json valuesJson = nlohmann::json::array();
        for (const auto& param : _params)
            valuesJson.push_back(param.toJson());

        jsonData[Schema::PARAMS_VEC_KEY] = valuesJson;

        return jsonData;
    }

    /**
     * @brief Get the key for the numeric vector parameter
     *
     * @tparam T
     * @tparam N
     * @return std::string The key for the numeric vector parameter
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    const std::string& NumericVecParam<T, N>::getKey() const
    {
        return _key;
    }

    /**
     * @brief Subscribe to changes in the dirty state of the numeric vector
     * parameter, this function subscribes to the dirty state changes of each
     * individual numeric parameter in the vector, when any of them changes, we
     * will emit a dirty change for the whole vector parameter, the provided
     * callback function will be called whenever the dirty state changes, the
     * user pointer can be used to pass additional data to the callback
     * function, the returned vector of Connection objects can be used to
     * unsubscribe from changes for each individual numeric parameter in the
     * vector by calling disconnect() on each Connection object or by letting
     * them go out of scope.
     *
     * @tparam T
     * @tparam N
     * @param func The callback function to call when the dirty state changes,
     * it should have the signature void(void* user, bool isDirty)
     * @param user A user-defined pointer that will be passed to the callback
     * function when it is called, this can be used to provide additional
     * context for the callback function
     * @return std::vector<Connection> An object representing the subscription,
     * this can be used to unsubscribe from changes by calling disconnect() on
     * it or by letting it go out of scope
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    std::vector<Connection> NumericVecParam<T, N>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        // subscribe to all individual numeric parameters in the vector, when
        // any of them changes, we will emit a dirty change for the whole vector
        // parameter
        std::vector<Connection> connections;
        for (auto& param : _params)
            connections.push_back(param.subscribeToDirty(func, user));

        return connections;
    }

    /**
     * @brief Get the X value of the numeric vector parameter, this is a
     * convenience function for accessing the first element of the vector when
     * N is 2 or 3, if N is greater than 3, this function will not be available
     *
     * @tparam T
     * @tparam N
     * @return const T& The X value of the numeric vector parameter (first
     * element of the vector)
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    const T& NumericVecParam<T, N>::x() const
    requires(N <= 3)
    {
        return _params[0].get();
    }

    /**
     * @brief Get the Y value of the numeric vector parameter, this is a
     * convenience function for accessing the second element of the vector when
     * N is 2 or 3, if N is greater than 3, this function will not be available
     *
     * @tparam T
     * @tparam N
     * @return const T& The Y value of the numeric vector parameter (second
     * element of the vector)
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    const T& NumericVecParam<T, N>::y() const
    requires(N <= 3)
    {
        return _params[1].get();
    }

    /**
     * @brief Get the Z value of the numeric vector parameter, this is a
     * convenience function for accessing the third element of the vector when
     * N is 3, if N is not equal to 3, this function will not be available
     *
     * @tparam T
     * @tparam N
     * @return const T& The Z value of the numeric vector parameter (third
     * element of the vector)
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    const T& NumericVecParam<T, N>::z() const
    requires(N == 3)
    {
        return _params[2].get();
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__