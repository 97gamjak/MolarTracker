#ifndef __SETTINGS__NUMERIC_PARAM_TPP__
#define __SETTINGS__NUMERIC_PARAM_TPP__

#include <cmath>
#include <expected>
#include <format>
#include <optional>
#include <string>

#include "numeric_param.hpp"
#include "param_error.hpp"

namespace settings
{
    /**
     * @brief Construct a new NumericParam object
     *
     * @tparam Derived
     * @tparam T
     * @param key
     * @param title
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    NumericParam<T>::NumericParam(std::string key, std::string title)
        : _core(std::move(key), std::move(title))
    {
    }

    /**
     * @brief Get the value of the numeric parameter
     *
     * @note This function returns the ParamCore object which contains the value
     * and other metadata about the parameter, to get the actual value, use the
     * get() function of the ParamCore object returned by this function.
     *
     * It is private to prevent direct access to the core object from outside
     * the class, and to enforce the use of the get() function for accessing the
     * value, which may include additional logic such as validation or applying
     * precision.
     *
     * @tparam Derived
     * @tparam T
     * @return const ParamCore<T>&
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    NumericParam<T>::NumericParam(ParamCore<T> core) : _core(std::move(core))
    {
    }

    /**
     * @brief Get the value of the numeric parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const T&
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    const ParamCore<T>& NumericParam<T>::core() const
    {
        return _core;
    }

    /**
     * @brief Get the value of the numeric parameter
     *
     * @tparam Derived
     * @tparam T
     * @return ParamCore<T>&
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    ParamCore<T>& NumericParam<T>::core()
    {
        return _core;
    }

    /**
     * @brief Set the value of the numeric parameter, this function checks if
     * the value is within the specified range before setting it, if the value
     * is out of range, an error is returned in the std::expected return type
     *
     * @tparam Derived
     * @tparam T
     * @param value
     * @return std::expected<void, ParamError>
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    std::expected<void, ParamError> NumericParam<T>::set(const T& value)
    {
        const auto valueToSet = _applyPrecision(value);

        const auto rangeCheck = _isWithinRange(valueToSet);
        if (!rangeCheck.has_value())
            return std::unexpected(rangeCheck.error());

        _core.set(valueToSet);
        return {};
    }

    /**
     * @brief Check if a value is within the specified range for the numeric
     * parameter
     *
     * @tparam Derived
     * @tparam T
     * @param value
     * @return std::expected<void, ParamError> if the value is out of range, an
     * error is returned in the std::expected return type
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    std::expected<void, ParamError> NumericParam<T>::_isWithinRange(
        const T& value
    ) const
    {
        const auto minCheck =
            !_minValue.has_value() || value >= _minValue.value();
        const auto maxCheck =
            !_maxValue.has_value() || value <= _maxValue.value();

        if (!minCheck || !maxCheck)
        {
            const auto errorMessage = std::format(
                "Value {} is out of range for parameter: {}, valid range is "
                "[{} - {}]",
                value,
                _core.getKey(),
                _minValue,
                _maxValue
            );
            return std::unexpected(ParamError(errorMessage));
        }
        return {};
    }

    /**
     * @brief Apply the specified precision to a value, this is used for
     * floating-point parameters to round the value to the specified number of
     * decimal places for validation and storage.
     *
     * @tparam Derived
     * @tparam T
     * @param value
     * @return T The value rounded to the specified precision if applicable,
     * otherwise the original value
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    T NumericParam<T>::_applyPrecision(const T& value) const
    {
        if constexpr (std::floating_point<T>)
        {
            if (_precision.has_value())
            {
                const auto exp    = static_cast<int>(_precision.value());
                const auto factor = std::pow(T{10}, exp);

                return std::round(value * factor) / factor;
            }
        }
        return value;
    }

    /**
     * @brief Set the minimum and maximum valid values for the numeric parameter
     *
     * @tparam Derived
     * @tparam T
     * @param minValue
     * @param maxValue
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    void NumericParam<T>::setLimits(const T& minValue, const T& maxValue)
    {
        _minValue = minValue;
        _maxValue = maxValue;
    }

    /**
     * @brief Set the minimum valid value for the numeric parameter
     *
     * @tparam Derived
     * @tparam T
     * @param minValue
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    void NumericParam<T>::setMinValue(const T& minValue)
    {
        _minValue = minValue;
    }

    /**
     * @brief Set the maximum valid value for the numeric parameter
     *
     * @tparam Derived
     * @tparam T
     * @param maxValue
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    void NumericParam<T>::setMaxValue(const T& maxValue)
    {
        _maxValue = maxValue;
    }

    /**
     * @brief Set the precision for floating-point numeric parameters, this
     * represents the number of decimal places to consider for validation and
     * storage, if the parameter is not a floating-point type, this function has
     * no effect
     *
     * @tparam Derived
     * @tparam T
     * @param decimalPlaces
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    void NumericParam<T>::setPrecision(size_t decimalPlaces)
    {
        if constexpr (std::floating_point<T>)
            _precision = decimalPlaces;
    }

    /**
     * @brief Serialize NumericParam to JSON
     *
     * @tparam T
     * @return nlohmann::json
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    nlohmann::json NumericParam<T>::toJson() const
    {
        auto jsonData                   = core().toJson();
        jsonData[Schema::MIN_VALUE_KEY] = _minValue;
        jsonData[Schema::MAX_VALUE_KEY] = _maxValue;
        jsonData[Schema::PRECISION_KEY] = _precision;
        return jsonData;
    }

    /**
     * @brief Deserialize NumericParam from JSON
     *
     * @tparam T
     * @param jsonData
     * @param param
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    void NumericParam<T>::fromJson(
        const nlohmann::json& jsonData,
        NumericParam<T>&      param
    )
    {
        ParamCore<T>::fromJson(jsonData, param.core());

        param._minValue =
            jsonData[Schema::MIN_VALUE_KEY].get<std::optional<T>>();
        param._maxValue =
            jsonData[Schema::MAX_VALUE_KEY].get<std::optional<T>>();
        param._precision =
            jsonData[Schema::PRECISION_KEY].get<std::optional<size_t>>();
    }

}   // namespace settings

#endif   // __SETTINGS__NUMERIC_PARAM_TPP__