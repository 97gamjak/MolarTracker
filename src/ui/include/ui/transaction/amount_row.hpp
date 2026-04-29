// amount_row.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__AMOUNT_ROW_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__AMOUNT_ROW_HPP__

#include <qwidget.h>

#include "config/finance.hpp"

namespace ui
{

    class AmountLineEdit;   // Forward declaration

    /**
     * @brief Reusable amount input widget
     *
     * Wraps an AmountLineEdit with sensible defaults for transaction forms
     * and re-exposes only the signals and accessors that owning widgets
     * need, so they never have to reach into the line edit directly.
     */
    class AmountRow : public QWidget
    {
        Q_OBJECT

       private:
        /// The line edit for entering the amount
        AmountLineEdit* _amountField;

       public:
        explicit AmountRow(QWidget* parent = nullptr);

        void setNDecimalPlaces(std::uint8_t places);

        [[nodiscard]] bool        isValid() const;
        [[nodiscard]] micro_units getAmount() const;

       signals:
        /**
         * @brief Emitted when the validity of the entered amount changes
         */
        void validityChanged();

        /**
         * @brief Emitted when the value of the entered amount changes
         */
        void valueChanged();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__AMOUNT_ROW_HPP__