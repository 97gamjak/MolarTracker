#ifndef __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__
#define __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__

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
    NumericVecParam<T, N>::NumericVecParam(
        const std::string& keyPrefix,
        const std::string& titlePrefix,
        const std::string& descriptionPrefix
    )
        : _key(keyPrefix), _title(titlePrefix), _description(descriptionPrefix)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            std::string indexSuffix = std::to_string(i);
            _params.emplace_back(
                keyPrefix + indexSuffix,
                titlePrefix + " " + indexSuffix,
                descriptionPrefix + " (index " + indexSuffix + ")"
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
     * @brief Commit the current values of the numeric vector parameter, this
     * function calls commit() on each individual numeric parameter in the
     * vector to save their current values as the baseline for dirty checking
     *
     * @tparam T
     * @tparam N
     */
    template <typename T, std::size_t N>
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
    std::string NumericVecParam<T, N>::getKey() const
    {
        return _key;
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__