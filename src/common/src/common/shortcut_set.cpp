#include "common/shortcut_set.hpp"

#include <QKeySequence>
#include <QList>

/**
 * @brief Constructs a ShortcutSet object.
 *
 * @param forWhat A descriptive string identifying the action/command this
 * shortcut is for.
 * @param context The context in which the shortcut is applicable (Default,
 * Global, Window).
 * @param mainShortcut The main keyboard shortcut associated with this
 * ShortcutSet.
 */
ShortcutSet::ShortcutSet(
    std::string     forWhat,
    ShortcutContext context,
    const Shortcut& mainShortcut
)
    : _forWhat{std::move(forWhat)},
      _context{context},
      _shortcuts{Set<Shortcut>{mainShortcut}}
{
}

/**
 * @brief Constructs a ShortcutSet object with multiple alternative shortcuts.
 *
 * @param forWhat A descriptive string identifying the action/command this
 * shortcut is for.
 * @param context The context in which the shortcut is applicable (Default,
 * Global, Window).
 * @param shortcuts A set of keyboard shortcuts associated with this
 * ShortcutSet.
 */
ShortcutSet::ShortcutSet(
    std::string          forWhat,
    ShortcutContext      context,
    const Set<Shortcut>& shortcuts
)
    : _forWhat{std::move(forWhat)}, _context{context}, _shortcuts{shortcuts}
{
}

/**
 * @brief Returns the descriptive string identifying the action/command this
 * shortcut is for.
 *
 * @return const std::string& The descriptive string.
 */
const std::string& ShortcutSet::getWhat() const { return _forWhat; }

/**
 * @brief Returns the set of keyboard shortcuts associated with this
 * ShortcutSet.
 *
 * @return const Set<Shortcut>& The set of keyboard shortcuts.
 */
const Set<Shortcut>& ShortcutSet::getShortcuts() const { return _shortcuts; }

/**
 * @brief Converts the ShortcutSet to a human-readable string representation.
 *
 * @return std::string The string representation of the ShortcutSet.
 */
std::string ShortcutSet::toString() const
{
    std::string result =
        _forWhat + " (" + ShortcutContextMeta::toString(_context) + "): ";

    for (const auto& shortcut : _shortcuts)
        result += shortcut.toString() + ", ";

    if (!result.empty())
    {
        result.pop_back();   // Remove the last space
        result.pop_back();   // Remove the last comma
    }
    return result;
}

/**
 * @brief Compares two ShortcutSet objects for equality.
 *
 * @param other The other ShortcutSet object to compare with.
 * @return true if both ShortcutSet objects are equal, false otherwise.
 */
bool ShortcutSet::operator==(const ShortcutSet& other) const
{
    return _forWhat == other._forWhat && _context == other._context &&
           _shortcuts == other._shortcuts;
}

/**
 * @brief Adds an alternative keyboard shortcut to the ShortcutSet.
 *
 * @param alternativeShortcut The alternative keyboard shortcut to add.
 */
void ShortcutSet::addAlternativeShortcut(const Shortcut& alternativeShortcut)
{
    _shortcuts.insert(alternativeShortcut);
}

/**
 * @brief Removes a keyboard shortcut from the ShortcutSet.
 *
 * @param shortcutToRemove The keyboard shortcut to remove.
 */
void ShortcutSet::removeShortcut(const Shortcut& shortcutToRemove)
{
    _shortcuts.remove(shortcutToRemove);
}

/**
 * @brief Updates an existing keyboard shortcut in the ShortcutSet with a new
 * shortcut.
 *
 * @param oldShortcut The existing keyboard shortcut to be replaced.
 * @param newShortcut The new keyboard shortcut to replace the old one.
 * @return true if the update was successful, false if the old shortcut was not
 * found.
 */
bool ShortcutSet::updateShortcutAt(
    const Shortcut& oldShortcut,
    const Shortcut& newShortcut
)
{
    if (!_shortcuts.contains(oldShortcut))
        return false;

    _shortcuts.remove(oldShortcut);
    _shortcuts.insert(newShortcut);
    return true;
}

/**
 * @brief Returns the context in which the shortcut is applicable (Default,
 * Global, Window).
 *
 * @return const ShortcutContext& The context of the ShortcutSet.
 */
const ShortcutContext& ShortcutSet::getContext() const { return _context; }

#ifdef __QT_ENABLED__

/**
 * @brief Converts the ShortcutSet to a list of QKeySequence objects, which can
 * be used in Qt applications for handling keyboard shortcuts.
 *
 * @return QList<QKeySequence> The list of QKeySequence objects.
 */
QList<QKeySequence> ShortcutSet::toQKeySequences() const
{
    QList<QKeySequence> sequences;
    for (const auto& shortcut : _shortcuts)
    {
        sequences.append(shortcut.toQKeySequence());
    }
    return sequences;
}

#endif   // __QT_ENABLED__