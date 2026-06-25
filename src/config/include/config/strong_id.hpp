#ifndef __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__
#define __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__

#include <cstdint>
#include <ostream>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief A strong typedef for IDs to prevent mixing different ID types
 *
 * @tparam Tag
 * @tparam Rep
 *
 */
template <typename Tag, typename Rep = std::int64_t>
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

    constexpr StrongId& operator--();
    constexpr StrongId  operator--(int);

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

template <typename StrongId, typename T>
using unorderedIdMap = std::unordered_map<StrongId, T, typename StrongId::Hash>;

/**
 * @brief A sequence of IDs, used to generate new unique IDs
 *
 * @tparam Id The ID type
 */
template <typename Id>
class IdSequence
{
    /// The next ID to be assigned -- starting from -2 because -1 is reserved
    /// for invalid IDs
    Id _next{-2};

   public:
    Id next();
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

template <typename T>
concept IsId = isStrongId_v<T>;

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__STRONG_ID_TPP__
#include "config/details/strong_id.tpp"
#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__STRONG_ID_TPP__

#endif   // __CONFIG__INCLUDE__CONFIG__STRONG_ID_HPP__