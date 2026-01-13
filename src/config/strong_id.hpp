#ifndef __CONFIG__STRONG_ID_HPP__
#define __CONFIG__STRONG_ID_HPP__

#include <compare>
#include <cstdint>

template <class Tag, class Rep = std::int64_t>
class StrongId final
{
   public:
    using rep_type = Rep;

    constexpr StrongId() = default;
    explicit constexpr StrongId(Rep value) : _value{value} {}

    [[nodiscard]] static constexpr StrongId invalid()
    {
        return StrongId{static_cast<Rep>(-1)};
    }

    [[nodiscard]] static constexpr StrongId from(Rep value)
    {
        return StrongId{value};
    }
    [[nodiscard]] constexpr Rep value() const { return _value; }

    [[nodiscard]] constexpr StrongId operator-() const
    {
        return StrongId{-_value};
    }

    bool operator==(const StrongId&) const  = default;
    auto operator<=>(const StrongId&) const = default;

    // For use in hash maps/sets
    struct Hash
    {
        std::size_t operator()(StrongId id) const
        {
            return std::hash<Rep>{}(id.value());
        }
    };

   private:
    Rep _value{};
};

#endif   // __CONFIG__STRONG_ID_HPP__