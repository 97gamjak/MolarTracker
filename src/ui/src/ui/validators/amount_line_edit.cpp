#include "ui/validators/amount_line_edit.hpp"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qregularexpression.h>

#include "logging/log_macros.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("UI.Validators.AmountLineEdit");

namespace ui
{
    AmountLineEdit::AmountLineEdit(QWidget* parent) : LineValidator(parent) {}

    QString AmountLineEdit::_getRequiredErrorMessage() const
    {
        return "Amount is required";
    }

    void AmountLineEdit::setNDecimalPlaces(std::uint8_t numberOfDecimalPlaces)
    {
        _nDecimalPlaces = numberOfDecimalPlaces;
    }

    void AmountLineEdit::setOnlyPositive(bool onlyPositive)
    {
        _onlyPositive = onlyPositive;
    }

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
