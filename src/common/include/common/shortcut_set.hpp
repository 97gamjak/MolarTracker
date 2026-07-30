#ifndef __COMMON__INCLUDE__COMMON__SHORTCUT_SET_HPP__
#define __COMMON__INCLUDE__COMMON__SHORTCUT_SET_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

#include "common/container/set.hpp"
#include "common/shortcut.hpp"

class QKeySequence;   // Forward declaration

template <typename T>
class QList;   // Forward declaration

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define SHORTCUT_CONTEXT_LIST(X) \
    X(Default)                   \
    X(Global)                    \
    X(Window)

MSTD_ENUM(ShortcutContext, std::uint8_t, SHORTCUT_CONTEXT_LIST)

/**
 * @brief Represents a set of keyboard shortcuts for a specific action or
 *        command. Each ShortcutSet can have multiple alternative shortcuts,
 *        and is associated with a context (Default, Global, Window).
 */
class ShortcutSet
{
   private:
    /// descriptive string identifying the action/command this shortcut is for
    std::string _forWhat;

    /// The context in which the shortcut is applicable
    ShortcutContext _context;

    /// The set of keyboard shortcuts associated with this ShortcutSet
    Set<Shortcut> _shortcuts;

   public:
    ShortcutSet(
        std::string     forWhat,
        ShortcutContext context,
        const Shortcut& mainShortcut
    );
    ShortcutSet(
        std::string          forWhat,
        ShortcutContext      context,
        const Set<Shortcut>& shortcuts
    );

    void addAlternativeShortcut(const Shortcut& alternativeShortcut);

    [[nodiscard]] const std::string&     getWhat() const;
    [[nodiscard]] const ShortcutContext& getContext() const;
    [[nodiscard]] const Set<Shortcut>&   getShortcuts() const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] bool operator==(const ShortcutSet& other) const;

    QList<QKeySequence> toQKeySequences() const;

    bool updateShortcutAt(
        const Shortcut& oldShortcut,
        const Shortcut& newShortcut
    );

    void removeShortcut(const Shortcut& shortcutToRemove);
};

#endif   // __COMMON__INCLUDE__COMMON__SHORTCUT_SET_HPP__