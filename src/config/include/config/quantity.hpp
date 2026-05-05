#ifndef __CONFIG__INCLUDE__CONFIG__QUANTITY_HPP__
#define __CONFIG__INCLUDE__CONFIG__QUANTITY_HPP__

#include <cstdint>
#include <string>

using micro_units = std::int64_t;

/**
 * @brief A compile-time integer power calculation
 *
 * @tparam N The exponent
 */
template <std::uint8_t N>
struct intPow
{
    /// The value of 10 raised to the power of N
    static constexpr std::uint64_t value = 10 * intPow<N - 1>::value;
};

/**
 * @brief Specialization of intPow for the case when N is 0
 *
 * @tparam N The exponent
 */
template <>
struct intPow<0>
{
    /// The value of 10 raised to the power of 0
    static constexpr std::uint64_t value = 1;
};

/**
 * @brief Represents a quantity of a financial instrument, with a fixed
 * precision.
 *
 */
class Quantity
{
   public:
    /// The precision of the quantity, in decimal places.
    static constexpr std::uint8_t precision = 8;
    /// The factor by which to multiply the quantity to convert to micro_units.
    static constexpr micro_units factor = intPow<precision>::value;

   private:
    /// The value of the quantity, stored in micro_units.
    micro_units _value;

   public:
    explicit Quantity(micro_units value);

    [[nodiscard]] double getValue() const;

    [[nodiscard]] micro_units toMicroUnits() const;

    [[nodiscard]]
    std::string toString() const;

    [[nodiscard]] bool operator>(const Quantity& other) const;

    template <typename T>
    friend bool operator>(const Quantity& lhs, const T& rhs);

    friend Quantity operator-(const Quantity& quantity);
};

[[nodiscard]] micro_units mulDiv(
    micro_units lhs,
    micro_units rhs,
    micro_units divisor
);

[[nodiscard]] micro_units mulDiv(micro_units lhs, Quantity rhs);

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__QUANTITY_TPP__
#include "details/quantity.tpp"
#endif

#endif   // __CONFIG__INCLUDE__CONFIG__QUANTITY_HPP__