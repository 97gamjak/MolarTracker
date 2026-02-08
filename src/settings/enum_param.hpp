#ifndef __SETTINGS__ENUM_PARAM_HPP__
#define __SETTINGS__ENUM_PARAM_HPP__

#include "mstd/type_traits.hpp"
#include "params_mixin.hpp"

namespace settings
{

    template <typename E>
    requires std::is_enum_v<E>
    class MSTDEnumParam : public ParamMixin<MSTDEnumParam<E>, E>
    {
        using EnumMeta = typename enum_meta<E>::type;
        MSTDEnumParam(std::string key, std::string title);

        [[nodiscard]] ParamCore<E>&       core();
        [[nodiscard]] const ParamCore<E>& core() const;

        void set(const E& value)
        {
            const auto& choices = EnumMeta::values();

            for (const auto& allowed : choices)
            {
                if (value == allowed)
                {
                    core().set(value);
                    return;
                }
            }
        }
    };

}   // namespace settings

#endif   // __SETTINGS__ENUM_PARAM_HPP__