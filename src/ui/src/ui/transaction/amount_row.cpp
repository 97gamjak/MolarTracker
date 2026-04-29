// amount_row.cpp
#include "ui/transaction/amount_row.hpp"

#include <qboxlayout.h>

#include "ui/validators/amount_line_edit.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

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

    void AmountRow::setNDecimalPlaces(int places)
    {
        _amountField->setNDecimalPlaces(places);
    }

    bool AmountRow::isValid() const { return _amountField->isValid(); }

    double AmountRow::getAmount() const { return _amountField->getAmount(); }

}   // namespace ui