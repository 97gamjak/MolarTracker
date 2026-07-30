#include "common/shortcut.hpp"

#ifdef __QT_ENABLED__
#include <qkeysequence.h>
#include <qlist.h>
#endif

/**
 * @brief Constructs a Shortcut object.
 *
 * @param modifiers The modifiers (Shift, Control, Alt, Super) associated with
 * this shortcut.
 * @param key The key associated with this shortcut (e.g., 'A', 'Enter', etc.).
 */
Shortcut::Shortcut(ShortcutModifier modifiers, std::uint64_t key)
    : _modifiers{modifiers}, _key{key}
{
}

/**
 * @brief Constructs a Shortcut object using an FKey.
 *
 * @param modifiers The modifiers (Shift, Control, Alt, Super) associated with
 * this shortcut.
 * @param key The FKey associated with this shortcut (e.g., F1, F2, etc.).
 */
Shortcut::Shortcut(ShortcutModifier modifiers, FKey key)
    : _modifiers{modifiers}, _key{static_cast<std::uint64_t>(key)}
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
    return _modifiers == other._modifiers && _key == other._key;
}

/**
 * @brief Returns the modifiers associated with this Shortcut.
 *
 * @return const ShortcutModifier& The modifiers (Shift, Control, Alt, Super).
 */
const ShortcutModifier& Shortcut::getModifiers() const { return _modifiers; }

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

    if (_key >= static_cast<uint64_t>(FKey::F1) &&
        _key <= static_cast<uint64_t>(FKey::F12))
    {
        result +=
            "F" + std::to_string(_key - static_cast<uint64_t>(FKey::F1) + 1);
    }
    else
    {
#ifdef __QT_ENABLED__
        // If Qt is enabled, we can use QKeySequence to get a more accurate
        // string representation of the key.
        QKeySequence seq  = toQKeySequence();
        result           += seq.toString().toStdString();
#else
        // If Qt is not enabled, we will just append the key as a character.
        result += static_cast<char>(_key);
#endif
    }

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

    auto key = static_cast<Qt::Key>(_key);

    if (_key >= static_cast<uint64_t>(FKey::F1) &&
        _key <= static_cast<uint64_t>(FKey::F12))
    {
        key = static_cast<Qt::Key>(
            Qt::Key_F1 + (_key - static_cast<int>(FKey::F1))
        );
    }

    return {QKeyCombination(mods, key)};
}

/**
 * @brief Creates a Shortcut object from a QKeySequence. This is useful for
 *        converting user input from Qt's key sequence representation to the
 *        internal Shortcut representation.
 *
 * @param seq The QKeySequence to convert.
 * @return Shortcut The constructed Shortcut object.
 */
Shortcut Shortcut::fromQKeySequence(const QKeySequence& seq)
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

    auto key = static_cast<std::uint64_t>(qtKey);

    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F12)
    {
        key = static_cast<std::uint64_t>(FKey::F1) +
              static_cast<std::uint64_t>(qtKey - Qt::Key_F1);
    }

    return Shortcut{modifiers, key};
}
#endif   // __QT_ENABLED__
