#ifndef __ORM__FIXED_STRING_HPP__
#define __ORM__FIXED_STRING_HPP__

#include <array>
#include <cstddef>
#include <string_view>

namespace orm
{
    /**
     * @brief A fixed-size string known at compile time
     *
     * @tparam Size
     */
    template <std::size_t Size>
    struct fixed_string
    {
        /// The characters of the string, stored in a std::array for fixed size
        std::array<char, Size> _data{};

        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE
        constexpr fixed_string(char const (&text)[Size]);

        [[nodiscard]] constexpr std::string_view view() const;
    };

    /**
     * @brief helper deduction guide for fixed_string
     *
     * @tparam Size
     */
    template <std::size_t Size>
    // NOLINTNEXTLINE
    fixed_string(char const (&)[Size]) -> fixed_string<Size>;
}   // namespace orm

#ifndef __ORM__FIXED_STRING_TPP__
#include "fixed_string.tpp"
#endif   // __ORM__FIXED_STRING_TPP__

#endif   // __ORM__FIXED_STRING_HPP__