#ifndef __UI__INCLUDE__UI__TRANSACTION__TIMESTAMP_FIELD_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TIMESTAMP_FIELD_HPP__

#include <qwidget.h>

#include "utils/timestamp.hpp"

class QDateEdit;     // Forward declaration
class QTimeEdit;     // Forward declaration
class QPushButton;   // Forward declaration

namespace ui
{

    /**
     * @brief Date and time pickers for a transaction timestamp
     *
     * Splits the input into a QDateEdit and a QTimeEdit so the user can
     * navigate each independently. Defaults to today at midnight on
     * construction and whenever the "Today" button is clicked.
     *
     * Converts to UTC on getTimestamp(), consistent with the
     * persistence layer convention.
     */
    class TimestampField : public QWidget
    {
        Q_OBJECT

       private:
        /// The field for selecting the transaction date
        QDateEdit* _dateEdit;
        /// The field for selecting the transaction time
        QTimeEdit* _timeEdit;
        /// The button for resetting the timestamp to the current date
        QPushButton* _todayButton;

       public:
        explicit TimestampField(QWidget* parent = nullptr);

        [[nodiscard]] Timestamp getTimestamp() const;

       private:
        void _resetToToday();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TIMESTAMP_FIELD_HPP__