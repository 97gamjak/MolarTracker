#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

class QKeySequence;   // Forward declaration

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define SHORTCUT_MODIFIER_LIST(X)    \
    X(None, 0U)                      \
    X(Shift, 1U << 0U)               \
    X(Control, 1U << 1U)             \
    X(Alt, 1U << 2U)                 \
    X(Super, 1U << 3U)               \
    X(ShiftControl, Shift | Control) \
    X(ControlAlt, Control | Alt)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FKEYS_LIST(X)  \
    X(F1, 0x01000030)  \
    X(F2, 0x01000031)  \
    X(F3, 0x01000032)  \
    X(F4, 0x01000033)  \
    X(F5, 0x01000034)  \
    X(F6, 0x01000035)  \
    X(F7, 0x01000036)  \
    X(F8, 0x01000037)  \
    X(F9, 0x01000038)  \
    X(F10, 0x01000039) \
    X(F11, 0x0100003A) \
    X(F12, 0x0100003B)

// This is a bitflag enum, so we want to use the underlying type
// NOLINTNEXTLINE(performance-enum-size, hicpp-signed-bitwise)
MSTD_ENUM_BITFLAG(ShortcutModifier, std::int64_t, SHORTCUT_MODIFIER_LIST)

// NOLINTNEXTLINE(performance-enum-size)
MSTD_ENUM(FKey, std::uint64_t, FKEYS_LIST)

/**
 * @brief Represents a keyboard shortcut, consisting of a key and optional
 *        modifiers (Shift, Control, Alt, Super). Each shortcut is associated
 *        with a specific action or command, identified by a descriptive string.
 */
class Shortcut
{
   private:
    /// The modifiers (Shift, Control, Alt, Super) associated with this shortcut
    ShortcutModifier _modifiers;

    /// The key associated with this shortcut (e.g., 'A', 'Enter', etc.)
    std::uint64_t _key;

   public:
    Shortcut(ShortcutModifier modifiers, std::uint64_t key);
    Shortcut(ShortcutModifier modifiers, FKey key);

    bool operator==(const Shortcut& other) const;

    [[nodiscard]] const ShortcutModifier& getModifiers() const;
    [[nodiscard]] const std::uint64_t&    getKey() const;
    [[nodiscard]] std::string             toString() const;

    [[nodiscard]] QKeySequence toQKeySequence() const;

    [[nodiscard]]
    static Shortcut fromQKeySequence(const QKeySequence& seq);

    /**
     * @brief Hash function for Shortcut, used for storing in hash-based
     *        containers like std::unordered_set or std::unordered_map.
     */
    struct Hash
    {
        /**
         * @brief Computes the hash value for a given Shortcut object.
         *
         * @param shortcut The Shortcut object to hash.
         * @return std::size_t The computed hash value.
         */
        std::size_t operator()(const Shortcut& shortcut) const
        {
            const auto hash1 =
                std::hash<ShortcutModifier>{}(shortcut._modifiers);

            const auto hash2 = std::hash<std::uint64_t>{}(shortcut._key);

            constexpr std::size_t shift = 1;

            return hash1 ^ (hash2 << shift);   // Combine the two hash values
        }
    };
};

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__