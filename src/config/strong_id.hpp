#ifndef __CONFIG__STRONG_ID_HPP__
#define __CONFIG__STRONG_ID_HPP__

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

    /// @cond DOXYGEN_IGNORE
    bool operator==(const StrongId&) const  = default;
    auto operator<=>(const StrongId&) const = default;
    /// @endcond

    template <class T, class U>
    friend std::ostream& operator<<(std::ostream& os, StrongId<Tag, Rep> id);
};

#include "strong_id.tpp"

#endif   // __CONFIG__STRONG_ID_HPP__