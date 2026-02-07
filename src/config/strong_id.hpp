#ifndef __CONFIG__STRONG_ID_HPP__
#define __CONFIG__STRONG_ID_HPP__

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>
#include <typeinfo>

/**
 * @brief A strong typedef for IDs to prevent mixing different ID types
 *
 * @tparam Tag
 * @tparam Rep
 */
template <class Tag, class Rep = std::int64_t>
class StrongId final
{
   public:
    using rep_type = Rep;

    constexpr StrongId() = default;
    explicit constexpr StrongId(Rep value) : _value{value} {}

    /**
     * @brief Get an invalid StrongId instance
     *
     * @return StrongId
     */
    [[nodiscard]] static constexpr StrongId invalid()
    {
        return StrongId{static_cast<Rep>(-1)};
    }

    /**
     * @brief Create a StrongId from the underlying representation
     *
     * @param value
     * @return StrongId
     */
    [[nodiscard]] static constexpr StrongId from(Rep value)
    {
        return StrongId{value};
    }

    /**
     * @brief Get the underlying value of the StrongId
     *
     * @return Rep
     */
    [[nodiscard]] constexpr Rep value() const { return _value; }

    /**
     * @brief Negate the StrongId value
     *
     * @return StrongId
     */
    [[nodiscard]] constexpr StrongId operator-() const
    {
        return StrongId{-_value};
    }

    // Comparison operators
    bool operator==(const StrongId&) const  = default;
    auto operator<=>(const StrongId&) const = default;

    /**
     * @brief Hash function for StrongId to be used in hash-based containers
     *
     */
    struct Hash
    {
        std::size_t operator()(StrongId id) const
        {
            return std::hash<Rep>{}(id.value());
        }
    };

    /**
     * @brief Output stream operator for StrongId
     *
     * @param os
     * @param id
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& os, StrongId id)
    {
        os << std::string(typeid(Tag).name()) << "(" << id.value() << ")";
        return os;
    }

   private:
    Rep _value{};
};

#endif   // __CONFIG__STRONG_ID_HPP__