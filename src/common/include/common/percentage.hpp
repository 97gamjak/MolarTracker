#ifndef __COMMON__INCLUDE__COMMON__PERCENTAGE_HPP__
#define __COMMON__INCLUDE__COMMON__PERCENTAGE_HPP__

#include <string>

/**
 * @brief Class representing a percentage value, providing a way to encapsulate
 * and manage percentage values in a type-safe manner.
 *
 */
class Percentage
{
   private:
    /// The base value for the percentage, used for calculations (e.g., 100 for
    /// percentage values)
    static constexpr size_t _base = 100;

    /// The underlying value of the percentage, stored as a double (e.g., 0.25
    /// for 25%)
    double _value = 0.0;

   public:
    explicit Percentage(double value);

    [[nodiscard]] std::string toString(size_t nDecimals = 2) const;
};

#endif   // __COMMON__INCLUDE__COMMON__PERCENTAGE_HPP__