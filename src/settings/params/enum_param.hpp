#ifndef __SETTINGS__ENUM_PARAM_HPP__
#define __SETTINGS__ENUM_PARAM_HPP__

#include <expected>

#include "mstd/type_traits.hpp"
#include "param_mixin.hpp"

namespace settings
{
    class ParamError;   // Forward declaration

    /**
     * @brief A setting parameter class for enum types, this class inherits from
     * ParamMixin to provide additional functionality for enum parameters
     *
     * @tparam E The enum type of the parameter
     */
    template <typename E>
    requires std::is_enum_v<E>
    class EnumParam : public ParamMixin<EnumParam<E>, E>
    {
       private:
        friend ParamMixin<EnumParam<E>, E>;
        ParamCore<E> _core;

        using EnumMeta = mstd::enum_meta_t<E>;
        EnumParam(std::string key, std::string title);

       public:
        std::expected<void, ParamError> set(const E& value);
    };

}   // namespace settings

#ifndef __SETTINGS__ENUM_PARAM_TPP__
#include "enum_param.tpp"
#endif   // __SETTINGS__ENUM_PARAM_TPP__

#endif   // __SETTINGS__ENUM_PARAM_HPP__