#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

class QKeySequence;   // Forward declaration

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define SHORTCUT_MODIFIER_LIST(X) \
    X(None, 0U)                   \
    X(Shift, 1U << 0U)            \
    X(Control, 1U << 1U)          \
    X(Alt, 1U << 2U)              \
    X(Super, 1U << 3U)

// This is a bitflag enum, so we want to use the underlying type
// NOLINTNEXTLINE(performance-enum-size)
MSTD_ENUM_BITFLAG(ShortcutModifier, std::int64_t, SHORTCUT_MODIFIER_LIST)

/**
 * @brief Represents a keyboard shortcut, consisting of a key and optional
 *        modifiers (Shift, Control, Alt, Super). Each shortcut is associated
 *        with a specific action or command, identified by a descriptive string.
 */
class Shortcut
{
   private:
    /// descriptive string identifying the action/command this shortcut is for
    std::string _forWhat;

    /// The modifiers (Shift, Control, Alt, Super) associated with this shortcut
    ShortcutModifier _modifiers;

    /// The key associated with this shortcut (e.g., 'A', 'Enter', etc.)
    std::uint64_t _key;

   public:
    Shortcut(std::string what, ShortcutModifier modifiers, std::uint64_t key);

    bool operator==(const Shortcut& other) const;

    [[nodiscard]]
    bool isKeyStrokeEqual(const Shortcut& other) const;

    [[nodiscard]] const std::string&      getWhat() const;
    [[nodiscard]] const ShortcutModifier& getModifiers() const;
    [[nodiscard]] const std::uint64_t&    getKey() const;
    [[nodiscard]] std::string             toString() const;

    [[nodiscard]] QKeySequence toQKeySequence() const;

    [[nodiscard]]
    static Shortcut fromQKeySequence(
        const std::string&  forWhat,
        const QKeySequence& seq
    );
};

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_HPP__