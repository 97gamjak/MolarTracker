#ifndef __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__
#define __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__

#include <cstdint>
#include <ostream>

/**
 * @brief A strong typedef for IDs to prevent mixing different ID types
 *
 * @tparam Tag
 * @tparam Rep
 *
 */
template <class Tag, class Rep = std::int64_t>
class StrongId final
{
   public:
    /**
     * @brief Hash function for StrongId to be used in hash-based containers
     *
     */
    struct Hash
    {
        /**
         * @brief Hash function for StrongId to be used in hash-based containers
         *
         * @param id
         * @return std::size_t
         */
        std::size_t operator()(StrongId id) const
        {
            return std::hash<Rep>{}(id.value());
        }
    };

   private:
    /// The underlying representation type of the StrongId.
    using rep_type = Rep;

    /// The underlying value of the StrongId.
    Rep _value{};

   public:
    constexpr StrongId() = default;
    explicit constexpr StrongId(Rep value);

    [[nodiscard]] static constexpr StrongId invalid();
    [[nodiscard]] static constexpr StrongId from(Rep value);
    [[nodiscard]] constexpr Rep             value() const;
    [[nodiscard]] constexpr StrongId        operator-() const;
    [[nodiscard]] constexpr std::string     toString() const;
    [[nodiscard]] constexpr bool            isValid() const;

    constexpr StrongId& operator++();
    constexpr StrongId  operator++(int);

    /// @cond DOXYGEN_IGNORE
    bool operator==(const StrongId&) const  = default;
    auto operator<=>(const StrongId&) const = default;
    /// @endcond

    template <class T, class U>
    friend std::ostream& operator<<(
        std::ostream&      output,
        StrongId<Tag, Rep> id
    );
};

template <class Tag>
struct isStrongId : std::false_type
{
};

template <class Tag, class Rep>
struct isStrongId<StrongId<Tag, Rep>> : std::true_type
{
};

template <typename T>
inline constexpr bool isStrongId_v = isStrongId<T>::value;

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__STRONG_ID_TPP__
#include "config/details/strong_id.tpp"
#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__STRONG_ID_TPP__

#endif   // __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__