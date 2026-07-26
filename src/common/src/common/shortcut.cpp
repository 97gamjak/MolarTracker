#include "common/shortcut.hpp"

#ifdef __QT_ENABLED__
#include <qkeysequence.h>
#endif

/**
 * @brief Constructs a Shortcut object.
 *
 * @param what Descriptive string identifying the action/command this shortcut
 * is for.
 * @param modifiers The modifiers (Shift, Control, Alt, Super) associated with
 * this shortcut.
 * @param key The key associated with this shortcut (e.g., 'A', 'Enter', etc.).
 */
Shortcut::Shortcut(
    std::string      what,
    ShortcutModifier modifiers,
    std::uint64_t    key
)
    : _forWhat{std::move(what)}, _modifiers{modifiers}, _key{key}
{
}

/**
 * @brief Compares two Shortcut objects for equality.
 *
 * @param other The other Shortcut object to compare with.
 * @return true if both Shortcut objects are equal, false otherwise.
 */
bool Shortcut::operator==(const Shortcut& other) const
{
    return _forWhat == other._forWhat && _modifiers == other._modifiers &&
           _key == other._key;
}

/**
 * @brief Checks if the key stroke (modifiers and key) of this Shortcut is equal
 *        to another Shortcut.
 *
 * @param other The other Shortcut object to compare with.
 * @return true if both Shortcuts have the same key stroke, false otherwise.
 */
bool Shortcut::isKeyStrokeEqual(const Shortcut& other) const
{
    return _modifiers == other._modifiers && _key == other._key;
}

/**
 * @brief Returns the modifiers associated with this Shortcut.
 *
 * @return const ShortcutModifier& The modifiers (Shift, Control, Alt, Super).
 */
const ShortcutModifier& Shortcut::getModifiers() const { return _modifiers; }

/**
 * @brief Returns the descriptive string identifying the action/command this
 *        Shortcut is for.
 *
 * @return const std::string& The descriptive string.
 */
const std::string& Shortcut::getWhat() const { return _forWhat; }

/**
 * @brief Returns the key associated with this Shortcut.
 *
 * @return const std::uint64_t& The key (e.g., 'A', 'Enter', etc.).
 */
const std::uint64_t& Shortcut::getKey() const { return _key; }

/**
 * @brief Converts this Shortcut to a human-readable string representation.
 *
 * @return std::string The string representation of the Shortcut.
 */
std::string Shortcut::toString() const
{
    std::string result;

    if ((_modifiers & ShortcutModifier::Control) == ShortcutModifier::Control)
        result += "Ctrl+";
    if ((_modifiers & ShortcutModifier::Shift) == ShortcutModifier::Shift)
        result += "Shift+";
    if ((_modifiers & ShortcutModifier::Alt) == ShortcutModifier::Alt)
        result += "Alt+";
    if ((_modifiers & ShortcutModifier::Super) == ShortcutModifier::Super)
        result += "Super+";

    result += std::to_string(_key);

    return result;
}

#ifdef __QT_ENABLED__
/**
 * @brief Converts this Shortcut to a QKeySequence, which can be used in Qt
 *        applications for handling keyboard shortcuts.
 *
 * @return QKeySequence The QKeySequence representation of the Shortcut.
 */
QKeySequence Shortcut::toQKeySequence() const
{
    Qt::KeyboardModifiers mods;
    if ((_modifiers & ShortcutModifier::Control) == ShortcutModifier::Control)
        mods |= Qt::ControlModifier;
    if ((_modifiers & ShortcutModifier::Shift) == ShortcutModifier::Shift)
        mods |= Qt::ShiftModifier;
    if ((_modifiers & ShortcutModifier::Alt) == ShortcutModifier::Alt)
        mods |= Qt::AltModifier;
    if ((_modifiers & ShortcutModifier::Super) == ShortcutModifier::Super)
        mods |= Qt::MetaModifier;

    return {QKeyCombination(mods, static_cast<Qt::Key>(_key))};
}

/**
 * @brief Creates a Shortcut object from a QKeySequence. This is useful for
 *        converting user input from Qt's key sequence representation to the
 *        internal Shortcut representation.
 *
 * @param forWhat Descriptive string identifying the action/command this
 * Shortcut is for.
 * @param seq The QKeySequence to convert.
 * @return Shortcut The constructed Shortcut object.
 */
Shortcut Shortcut::fromQKeySequence(
    const std::string&  forWhat,
    const QKeySequence& seq
)
{
    const auto keyCombination = seq[0];
    const auto qtModifiers    = keyCombination.keyboardModifiers();
    const auto qtKey          = keyCombination.key();

    ShortcutModifier modifiers = ShortcutModifier::None;
    if ((qtModifiers & Qt::ControlModifier) == Qt::ControlModifier)
        modifiers |= ShortcutModifier::Control;
    if ((qtModifiers & Qt::ShiftModifier) == Qt::ShiftModifier)
        modifiers |= ShortcutModifier::Shift;
    if ((qtModifiers & Qt::AltModifier) == Qt::AltModifier)
        modifiers |= ShortcutModifier::Alt;
    if ((qtModifiers & Qt::MetaModifier) == Qt::MetaModifier)
        modifiers |= ShortcutModifier::Super;

    return Shortcut{forWhat, modifiers, static_cast<std::uint64_t>(qtKey)};
}
#endif   // __QT_ENABLED__