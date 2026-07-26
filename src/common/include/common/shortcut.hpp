#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

#define SHORTCUT_MODIFIER_LIST(X) \
    X(Shift, 1U << 0U)            \
    X(Control, 1U << 1U)          \
    X(Alt, 1U << 2U)              \
    X(Super, 1U << 3U)

MSTD_ENUM_BITFLAG(ShortcutModifier, std::int64_t, SHORTCUT_MODIFIER_LIST)

class Shortcut
{
   private:
    std::string _what;

    ShortcutModifier _modifiers;
    std::uint64_t    _key;

   public:
    Shortcut(ShortcutModifier modifiers, std::uint64_t key, std::string what);
};

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__