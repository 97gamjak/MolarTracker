#ifndef __ORM__INCLUDE__ORM__FIXED_STRING_HPP__
#define __ORM__INCLUDE__ORM__FIXED_STRING_HPP__

#include <array>
#include <cstddef>
#include <format>
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

        explicit operator std::string() const;
        explicit operator std::string_view() const;
    };

    /**
     * @brief helper deduction guide for fixed_string
     *
     * @tparam Size
     */
    template <std::size_t Size>
    // NOLINTNEXTLINE
    fixed_string(char const (&)[Size]) -> fixed_string<Size>;

    template <std::size_t N>
    inline std::string operator+(
        const std::string&          lhs,
        const orm::fixed_string<N>& rhs
    )
    {
        return lhs + std::string(rhs);
    }

    template <std::size_t N>
    inline std::string operator+(
        const orm::fixed_string<N>& lhs,
        const std::string&          rhs
    )
    {
        return std::string(lhs) + rhs;
    }

    template <std::size_t N>
    std::string& operator+=(std::string& lhs, const orm::fixed_string<N>& rhs)
    {
        lhs.append(rhs._data.data(), N - 1);
        return lhs;
    }

}   // namespace orm

// NOLINTBEGIN(cert-dcl58-cpp)
/**
 * @brief Specialization of std::formatter for orm::fixed_string to allow
 * formatting fixed_string with std::format
 *
 * @tparam N
 */
/// @cond DOXYGEN_IGNORE
template <size_t N>
struct std::formatter<orm::fixed_string<N>> : std::formatter<std::string_view>
{
    auto format(
        const orm::fixed_string<N>& string,
        std::format_context&        ctx
    ) const
    {
        return std::formatter<std::string_view>::format(
            std::string_view(string),
            ctx
        );
    }
};
/// @endcond
// NOLINTEND(cert-dcl58-cpp)

#ifndef __ORM__INCLUDE__ORM__FIXED_STRING_TPP__
#include "fixed_string.tpp"
#endif   // __ORM__INCLUDE__ORM__FIXED_STRING_TPP__

#endif   // __ORM__INCLUDE__ORM__FIXED_STRING_HPP__