// amount_row.cpp
#include "ui/transaction/amount_row.hpp"

#include <qboxlayout.h>

#include <cstdint>

#include "config/finance.hpp"
#include "ui/validators/amount_line_edit.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Amount Row:: Amount Row object
     *
     * @param parent The parent widget for the amount row
     */
    AmountRow::AmountRow(QWidget* parent)
        : QWidget(parent), _amountField(makeQChild<AmountLineEdit>(this))
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_amountField);
        setLayout(layout);

        _amountField->setRequired(true);
        _amountField->setOnlyPositive(true);
        _amountField->setText("0");

        connect(
            _amountField,
            &AmountLineEdit::validityChanged,
            this,
            &AmountRow::validityChanged
        );
        connect(
            _amountField,
            &AmountLineEdit::valueChanged,
            this,
            &AmountRow::valueChanged
        );
    }

    /**
     * @brief Set the number of decimal places allowed in the amount row, this
     * will configure the underlying AmountLineEdit to allow the specified
     * number of decimal places, ensuring that the user can only enter valid
     * amounts with the appropriate level of precision.
     *
     * @param places The number of decimal places to allow in the amount input
     */
    void AmountRow::setNDecimalPlaces(std::uint8_t places)
    {
        _amountField->setNDecimalPlaces(places);
    }

    /**
     * @brief Check if the entered amount is valid, this will return true if the
     * input in the amount field is valid according to the validation rules
     * defined in the AmountLineEdit (e.g., required, only positive, etc.), and
     * false otherwise, allowing the owning widget to easily check if the amount
     * entered by the user is valid before proceeding with any actions that
     * depend on a valid amount.
     *
     * @return true if the entered amount is valid, false otherwise
     */
    bool AmountRow::isValid() const { return _amountField->isValid(); }

    /**
     * @brief Get the entered amount as a double, this will retrieve the value
     * from the underlying AmountLineEdit and return it as a double, allowing
     * the owning widget to easily access the entered amount for use in creating
     * transactions or performing calculations.
     *
     * @return micro_units The entered amount as a double
     */
    micro_units AmountRow::getAmount() const
    {
        return _amountField->getAmount();
    }

}   // namespace ui