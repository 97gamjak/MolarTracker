#ifndef __CONFIG__STRONG_ID_TPP__
#define __CONFIG__STRONG_ID_TPP__

#include "strong_id.hpp"

/**
 * @brief Construct a new StrongId object from the underlying representation
 *
 * @tparam Tag
 * @tparam Rep
 * @param value
 */
template <class Tag, class Rep>
constexpr StrongId<Tag, Rep>::StrongId(Rep value) : _value{value}
{
}

/**
 * @brief Get an invalid StrongId instance
 *
 * @tparam Tag
 * @tparam Rep
 * @return StrongId
 */
template <class Tag, class Rep>
constexpr StrongId<Tag, Rep> StrongId<Tag, Rep>::invalid()
{
    return StrongId<Tag, Rep>{static_cast<Rep>(-1)};
}

/**
 * @brief Create a StrongId from the underlying representation
 *
 * @tparam Tag
 * @tparam Rep
 * @param value
 * @return StrongId
 */
template <class Tag, class Rep>
constexpr StrongId<Tag, Rep> StrongId<Tag, Rep>::from(Rep value)
{
    return StrongId<Tag, Rep>{value};
}

/**
 * @brief Get the underlying value of the StrongId
 *
 * @return Rep
 */
template <class Tag, class Rep>
constexpr Rep StrongId<Tag, Rep>::value() const
{
    return _value;
}

/**
 * @brief Negate the StrongId value
 *
 * @return StrongId
 */
template <class Tag, class Rep>
constexpr StrongId<Tag, Rep> StrongId<Tag, Rep>::operator-() const
{
    return StrongId<Tag, Rep>{-_value};
}

/**
 * @brief Output stream operator for StrongId
 *
 * @param output
 * @param id
 * @return std::ostream&
 */
template <class Tag, class Rep>
std::ostream& operator<<(std::ostream& output, StrongId<Tag, Rep> id)
{
    output << std::string(typeid(Tag).name()) << "(" << id.value() << ")";
    return output;
}

#endif   // __CONFIG__STRONG_ID_TPP__