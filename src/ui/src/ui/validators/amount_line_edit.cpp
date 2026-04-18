#include "ui/validators/amount_line_edit.hpp"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qregularexpression.h>

#include "logging/log_macros.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("UI.Validators.AmountLineEdit");

namespace ui
{
    /**
     * @brief Construct a new Amount Line Edit:: Amount Line Edit object
     *
     * @param parent
     */
    AmountLineEdit::AmountLineEdit(QWidget* parent) : LineValidator(parent) {}

    /**
     * @brief Get the required error message for the amount line edit, this will
     * be displayed when the input is empty and the field is required, providing
     * feedback to the user that they need to enter a value in this field.
     *
     * @return QString The error message to display when the input is empty and
     * the field is required.
     */
    QString AmountLineEdit::_getRequiredErrorMessage() const
    {
        return "Amount is required";
    }

    /**
     * @brief set the number of decimal places allowed in the amount line edit
     *
     * @param numberOfDecimalPlaces The number of decimal places to allow in the
     * input.
     */
    void AmountLineEdit::setNDecimalPlaces(std::uint8_t numberOfDecimalPlaces)
    {
        _nDecimalPlaces = numberOfDecimalPlaces;
    }

    /**
     * @brief Set whether the amount line edit should only accept positive
     * values, this will restrict the input to positive numbers and provide
     * feedback to the user if they enter a negative value when only positive
     * values are allowed.
     *
     * @param onlyPositive A boolean indicating whether to restrict the input to
     * positive values, if true, the line edit will only accept positive numbers
     * and will display an error message if a negative value is entered.
     */
    void AmountLineEdit::setOnlyPositive(bool onlyPositive)
    {
        _onlyPositive = onlyPositive;
    }

    /**
     * @brief Validate the non-empty input for the amount line edit, this will
     * check if the input is a valid amount format and optionally if it is
     * positive, providing feedback to the user if the input is invalid.
     *
     * @param trimmedText The input text to validate, this is the text from the
     * line edit with leading and trailing whitespace removed, ensuring that we
     * are validating the actual content of the input without any extra spaces.
     * @return std::pair<bool, QString> A pair where the first element indicates
     * whether the input is valid, and the second element is an error message to
     * display if the input is invalid (or an empty string if the input is
     * valid).
     */
    std::pair<bool, QString> AmountLineEdit::_validateNonEmpty(
        QStringView trimmedText
    ) const
    {
        // Regular expression to match a valid amount with the specified number
        // of decimal places, this allows us to validate that the input is a
        // valid amount format and has the correct number of decimal places for
        // the currency
        const QRegularExpression amountRegex(
            QString::fromStdString(
                R"(^\d+(\.\d{0,)" + std::to_string(_nDecimalPlaces) + "})?$"
            )
        );

        if (!amountRegex.match(trimmedText).hasMatch())
        {
            return {false, "Invalid amount format"};
        }

        if (_onlyPositive && trimmedText.startsWith('-'))
        {
            return {false, "Amount must be positive"};
        }

        return {true, ""};
    }

}   // namespace ui
