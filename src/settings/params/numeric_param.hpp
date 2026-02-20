#ifndef __SETTINGS__NUMERIC_PARAM_HPP__
#define __SETTINGS__NUMERIC_PARAM_HPP__

#include <expected>
#include <optional>

#include "param_core.hpp"
#include "param_error.hpp"
#include "param_mixin.hpp"

namespace settings
{
    /**
     * @brief Schema for numeric parameters, this struct defines the keys for
     * the JSON representation of numeric parameters
     */
    class NumericParamSchema
    {
       public:
        /// key for the minimum value of the numeric parameter
        static constexpr const char* MIN_VALUE_KEY = "minValue";

        /// key for the maximum value of the numeric parameter
        static constexpr const char* MAX_VALUE_KEY = "maxValue";

        /// key for the precision of the numeric parameter
        static constexpr const char* PRECISION_KEY = "precision";
    };

    /**
     * @brief A setting parameter class for numeric types, this class
     * inherits from ParamMixin to provide additional functionality for
     * numeric parameters
     *
     * @tparam T The numeric type of the parameter
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    class NumericParam : public ParamMixin<NumericParam<T>, T>
    {
       private:
        /// type alias for NumericParamSchema
        using Schema = NumericParamSchema;
        /// friend declaration to allow ParamMixin to access private members of
        /// NumericParam, this is necessary because ParamMixin needs to access
        /// the _core member of NumericParam to implement the functionality for
        /// numeric parameters
        friend ParamMixin<NumericParam<T>, T>;

        /// The core container for the numeric parameter
        ParamCore<T> _core;

        /// The minimum value for the numeric parameter, if set, the parameter
        /// value must be greater than or equal to this value
        std::optional<T> _minValue = std::nullopt;

        /// The maximum value for the numeric parameter, if set, the parameter
        /// value must be less than or equal to this value
        std::optional<T> _maxValue = std::nullopt;

        /// For floating-point types, this represents the number of decimal
        /// places to consider for validation
        std::optional<size_t> _precision;

       public:
        NumericParam(std::string key, std::string title);
        NumericParam(
            std::string key,
            std::string title,
            std::string description
        );

        [[nodiscard]] std::expected<void, ParamError> set(const T& value);
        void setLimits(const T& minValue, const T& maxValue);
        void setMinValue(const T& minValue);
        void setMaxValue(const T& maxValue);
        void setPrecision(size_t decimalPlaces);

        [[nodiscard]] nlohmann::json toJson() const;
        static void fromJson(const nlohmann::json& j, NumericParam<T>& param);

       private:
        NumericParam(ParamCore<T> core);

        [[nodiscard]] std::expected<void, ParamError> _isWithinRange(
            const T& v
        ) const;
        [[nodiscard]] T _applyPrecision(const T& v) const;
    };

}   // namespace settings

#ifndef __SETTINGS__NUMERIC_PARAM_TPP__
#include "numeric_param.tpp"
#endif   // __SETTINGS__NUMERIC_PARAM_TPP__

#endif   // __SETTINGS__NUMERIC_PARAM_HPP__