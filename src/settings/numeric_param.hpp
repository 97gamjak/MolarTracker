#ifndef __SETTINGS__NUMERIC_PARAM_HPP__
#define __SETTINGS__NUMERIC_PARAM_HPP__

#include <expected>
#include <optional>

#include "nlohmann/json.hpp"
#include "param_mixin.hpp"

namespace settings
{
    class ParamError;   // Forward declaration

    class NumericParamSchema
    {
       public:
        static constexpr const char* MIN_VALUE_KEY = "minValue";
        static constexpr const char* MAX_VALUE_KEY = "maxValue";
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
        using Schema = NumericParamSchema;

        ParamCore<T>     _core;
        std::optional<T> _minValue = std::nullopt;
        std::optional<T> _maxValue = std::nullopt;

        // For floating-point types, this represents the number of decimal
        // places to consider for validation
        std::optional<size_t> _precision;

       public:
        NumericParam(std::string key, std::string title);

        [[nodiscard]] ParamCore<T>&       core();
        [[nodiscard]] const ParamCore<T>& core() const;

        std::expected<void, ParamError> set(const T& value);
        void setLimits(const T& minValue, const T& maxValue);
        void setMinValue(const T& minValue);
        void setMaxValue(const T& maxValue);
        void setPrecision(size_t decimalPlaces);

        nlohmann::json         toJson() const;
        static NumericParam<T> fromJson(const nlohmann::json& j);

       private:
        NumericParam(ParamCore<T> core);

        std::expected<void, ParamError> _isWithinRange(const T& v) const;
        T                               _applyPrecision(const T& v) const;
    };

}   // namespace settings

#ifndef __SETTINGS__NUMERIC_PARAM_TPP__
#include "numeric_param.tpp"
#endif   // __SETTINGS__NUMERIC_PARAM_TPP__

#endif   // __SETTINGS__NUMERIC_PARAM_HPP__