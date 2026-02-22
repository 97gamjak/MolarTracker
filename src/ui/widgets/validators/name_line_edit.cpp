#include "name_line_edit.hpp"

namespace ui
{
    /**
     * @brief Construct a new Name Line Edit:: Name Line Edit object
     *
     * @param parent Parent widget
     */
    NameLineEdit::NameLineEdit(QWidget* parent) : LineValidator(parent) {}

    /**
     * @brief Get the error message to be shown when the field is required but
     * empty, for name fields we want to show a specific message indicating that
     * a name is required.
     *
     * @return QString the error message to show when the field is required but
     * empty
     */
    QString NameLineEdit::_getRequiredErrorMessage() const
    {
        return "A name is required.";
    }

    /**
     * @brief Validate the given non-empty text according to the rules for valid
     * names. For name fields, we want to allow only certain characters (e.g.
     * letters, numbers, underscores, hyphens, etc.) and disallow any characters
     * that are not valid in names.
     *
     * @param trimmedText the text to validate, guaranteed to be non-empty and
     * trimmed of whitespace
     * @return std::pair<bool, QString> a pair of (isValid, errorMessage)
     */
    std::pair<bool, QString> NameLineEdit::_validateNonEmpty(
        QStringView trimmedText
    ) const
    {
        const auto text = trimmedText.toString();

        if (!_allowedCharsRegex.match(text).hasMatch())
        {
            return {
                false,
                "Names can only contain letters, numbers, underscores, "
                "hyphens, and periods."
            };
        }

        return {true, ""};
    }

}   // namespace ui