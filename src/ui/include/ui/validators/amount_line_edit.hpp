#ifndef __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__
#define __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__

#include "config/finance.hpp"
#include "line_validator.hpp"

namespace ui
{
    /**
     * @brief A QLineEdit for entering monetary amounts, with validation for
     * numeric input, optional positivity constraint, and configurable decimal
     * places.
     *
     * This class extends LineValidator to provide specific validation logic for
     * monetary amounts. It ensures that the input is a valid number, optionally
     * restricts it to positive values, and allows configuring the number of
     * decimal places. It also provides appropriate error messages when the
     * input is invalid.
     */
    class AmountLineEdit : public LineValidator
    {
        Q_OBJECT

       private:
        /// The number of decimal places allowed in the input
        std::uint8_t _nDecimalPlaces = 2;
        /// A flag indicating whether to restrict the input to positive values
        bool _onlyPositive = false;

       public:
        explicit AmountLineEdit(QWidget* parent);

        void setNDecimalPlaces(std::uint8_t numberOfDecimalPlaces);
        void setOnlyPositive(bool onlyPositive);

        [[nodiscard]] micro_units getAmount() const;

       protected:
        [[nodiscard]] QString _getRequiredErrorMessage() const override;
        [[nodiscard]] std::pair<bool, QString> _validateNonEmpty(
            QStringView trimmedText
        ) const override;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__