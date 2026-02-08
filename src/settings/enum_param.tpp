#ifndef __SETTINGS__ENUM_PARAM_TPP__
#define __SETTINGS__ENUM_PARAM_TPP__

#include "enum_param.hpp"
#include "param_error.hpp"

namespace settings
{
    /**
     * @brief Get the value of the enum parameter
     *
     * @tparam Derived
     * @tparam T
     * @return const T&
     */
    template <typename E>
    requires std::is_enum_v<E>
    const ParamCore<E>& EnumParam<E>::core() const
    {
        return _core;
    }

    /**
     * @brief Get the value of the enum parameter
     *
     * @tparam Derived
     * @tparam T
     * @return ParamCore<T>&
     */
    template <typename E>
    requires std::is_enum_v<E>
    ParamCore<E>& EnumParam<E>::core()
    {
        return _core;
    }

    /**
     * @brief Set the value of the enum parameter
     *
     * @tparam Derived
     * @tparam T
     * @param value
     * @return std::expected<void, ParamError>
     */
    template <typename E>
    requires std::is_enum_v<E>
    std::expected<void, ParamError> EnumParam<E>::set(const E& value)
    {
        for (const auto& validValue : EnumMeta::values_view())
        {
            if (validValue == value)
            {
                _core.set(value);
                return {};
            }
        }

        const auto errorMessage =
            "Invalid value for enum parameter: " +
            std::to_string(static_cast<std::underlying_type_t<E>>(value));

        return std::unexpected(ParamError{errorMessage});
    }

}   // namespace settings

#endif   // __SETTINGS__ENUM_PARAM_TPP__