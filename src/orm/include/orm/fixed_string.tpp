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

    template <std::size_t Size>
    fixed_string<Size>::operator std::string() const
    {
        return std::string{_data.data(), Size - 1};
    }

    template <std::size_t Size>
    fixed_string<Size>::operator std::string_view() const
    {
        return std::string_view{_data.data(), Size - 1};
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__FIXED_STRING_TPP__