#ifndef __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__
#define __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__

#include "line_validator.hpp"

namespace ui
{
    class AmountLineEdit : public LineValidator
    {
        Q_OBJECT

       private:
        std::uint8_t _nDecimalPlaces = 2;
        bool         _onlyPositive   = false;

       public:
        explicit AmountLineEdit(QWidget* parent);

        void setNDecimalPlaces(std::uint8_t numberOfDecimalPlaces);
        void setOnlyPositive(bool onlyPositive);

       protected:
        [[nodiscard]] QString _getRequiredErrorMessage() const override;
        [[nodiscard]] std::pair<bool, QString> _validateNonEmpty(
            QStringView trimmedText
        ) const override;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__VALIDATORS__AMOUNT_LINE_EDIT_HPP__