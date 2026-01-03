#ifndef __CONFIG__STRONG_ID_HPP__
#define __CONFIG__STRONG_ID_HPP__

#include <compare>
#include <cstdint>

template <class Tag, class Rep = std::int64_t>
class StrongId final
{
   public:
    using rep_type = Rep;

    constexpr StrongId() noexcept = default;
    explicit constexpr StrongId(Rep value) noexcept : _value{value} {}

    [[nodiscard]] static constexpr StrongId invalid() noexcept
    {
        return StrongId{static_cast<Rep>(-1)};
    }

    [[nodiscard]] static constexpr StrongId from(Rep value) noexcept
    {
        return StrongId{value};
    }
    [[nodiscard]] constexpr Rep value() const noexcept { return _value; }

    friend constexpr bool operator==(StrongId, StrongId) noexcept  = default;
    friend constexpr auto operator<=>(StrongId, StrongId) noexcept = default;

   private:
    Rep _value{};
};

#endif   // __CONFIG__STRONG_ID_HPP__