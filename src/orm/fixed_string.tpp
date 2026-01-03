#ifndef __ORM__FIXED_STRING_TPP__
#define __ORM__FIXED_STRING_TPP__

#include <array>
#include <cstddef>
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
    constexpr fixed_string<Size>::fixed_string(char const (&text)[Size])
    {
        for (std::size_t index = 0; index < Size; ++index)
        {
            _data[index] = text[index];
        }
    }

    /**
     * @brief get a string_view of the fixed string
     *
     * @tparam Size
     * @return constexpr std::string_view
     */
    template <std::size_t Size>
    constexpr std::string_view fixed_string<Size>::view() const noexcept
    {
        return std::string_view{_data.data(), Size - 1};
    }

}   // namespace orm

#endif   // __ORM__FIXED_STRING_TPP__