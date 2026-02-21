#include "email_line_edit.hpp"

#include <QRegularExpression>

namespace ui
{
    /**
     * @brief Construct a new Email Line Edit:: Email Line Edit object
     *
     * @param parent Parent widget
     */
    EmailLineEdit::EmailLineEdit(QWidget* parent) : LineValidator(parent) {}

    /**
     * @brief Get the error message to be shown when the field is required but
     * empty, for email fields we want to show a specific message indicating
     * that an email address is required.
     *
     * @return QString the error message to show when the field is required but
     * empty
     */
    QString EmailLineEdit::_getRequiredErrorMessage() const
    {
        return "An email address is required.";
    }

    /**
     * @brief Validate the given non-empty text according to the rules for valid
     * email addresses. For email fields, we want to check that the input
     * contains an "@" symbol and is in a valid email format.
     *
     * @param trimmedText the text to validate, guaranteed to be non-empty and
     * trimmed of whitespace
     * @return std::pair<bool, QString> a pair of (isValid, errorMessage)
     */
    std::pair<bool, QString> EmailLineEdit::_validateNonEmpty(
        QStringView trimmedText
    ) const
    {
        const auto text = trimmedText.toString();

        if (!text.contains('@'))
        {
            return {false, "Email addresses must contain an '@' symbol."};
        }

        // Simple regex for basic email validation, this can be improved for
        // more strict validation if needed
        static const QRegularExpression emailRegex(
            R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"
        );

        if (!emailRegex.match(text).hasMatch())
        {
            return {false, "Please enter a valid email address."};
        }

        return {true, ""};
    }

}   // namespace ui