#ifndef __SETTINGS__PARAMS__ENUM_PARAM_HPP__
#define __SETTINGS__PARAMS__ENUM_PARAM_HPP__

#include <expected>

#include "mstd/type_traits.hpp"
#include "nlohmann/json.hpp"
#include "param_core.hpp"
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
        /// friend declaration to allow ParamMixin to access private members of
        /// EnumParam, this is necessary because ParamMixin needs to access the
        /// _core member of EnumParam to implement the functionality for enum
        /// parameters
        friend ParamMixin<EnumParam<E>, E>;

        /// The core implementation of the parameter
        ParamCore<E> _core;

        /// type alias for the enum metadata
        using EnumMeta = mstd::enum_meta_t<E>;

       public:
        EnumParam(std::string key, std::string title, std::string description);

        [[nodiscard]] nlohmann::json toJson() const;
        static void fromJson(const nlohmann::json& j, EnumParam<E>& param);

        std::expected<void, ParamError> set(const E& value);
    };

}   // namespace settings

#ifndef __SETTINGS__ENUM_PARAM_TPP__
#include "enum_param.tpp"
#endif   // __SETTINGS__ENUM_PARAM_TPP__

#endif   // __SETTINGS__PARAMS__ENUM_PARAM_HPP__