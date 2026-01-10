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

    [[nodiscard]] constexpr StrongId operator-() const noexcept
    {
        return StrongId{-_value};
    }

    bool operator==(const StrongId&) const noexcept  = default;
    auto operator<=>(const StrongId&) const noexcept = default;

    // For use in hash maps/sets
    struct Hash
    {
        std::size_t operator()(StrongId id) const noexcept
        {
            return std::hash<Rep>{}(id.value());
        }
    };

   private:
    Rep _value{};
};

#endif   // __CONFIG__STRONG_ID_HPP__