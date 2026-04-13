#ifndef __ORM__INCLUDE__ORM__FIXED_STRING_TPP__
#define __ORM__INCLUDE__ORM__FIXED_STRING_TPP__

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

#include "fixed_string.hpp"

namespace orm
{
    /**
     * @brief Construct a new fixed_string<Size>:: fixed_string object
     *
     * @tparam Size
     * @param text
     */
    template <std::size_t Size>
    // NOLINTNEXTLINE
    constexpr fixed_string<Size>::fixed_string(char const (&text)[Size])
    {
        for (std::size_t index = 0; index < Size; ++index)
            _data[index] = text[index];
    }

    /**
     * @brief Convert the fixed_string to a std::string
     *
     * @tparam Size
     * @return std::string
     */
    template <std::size_t Size>
    fixed_string<Size>::operator std::string() const
    {
        return std::string{_data.data(), Size - 1};
    }

    /**
     * @brief Convert the fixed_string to a std::string_view
     *
     * @tparam Size
     * @return std::string_view
     */
    template <std::size_t Size>
    fixed_string<Size>::operator std::string_view() const
    {
        return std::string_view{_data.data(), Size - 1};
    }

    template <std::size_t SizeLHS, std::size_t SizeRHS>
    constexpr bool operator==(
        const fixed_string<SizeLHS>& lhs,
        const char (&rhs)[SizeRHS]
    )
    {
        if constexpr (SizeLHS == SizeRHS)
            return lhs._data == std::to_array(rhs);
        else
            return false;
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__FIXED_STRING_TPP__