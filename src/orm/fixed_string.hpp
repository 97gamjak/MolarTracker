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
       private:
        std::array<char, Size> _data{};

       public:
        constexpr fixed_string(char const (&text)[Size]);

        [[nodiscard]] constexpr std::string_view view() const noexcept;
    };

    // Class template argument deduction guide
    template <std::size_t Size>
    fixed_string(char const (&)[Size]) -> fixed_string<Size>;
}   // namespace orm

#endif   // __ORM__FIXED_STRING_HPP__