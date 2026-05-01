#include "ui/transaction/timestamp_field.hpp"

#include <qboxlayout.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qpushbutton.h>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Timestamp Field:: Timestamp Field object
     *
     * @param parent
     */
    TimestampField::TimestampField(QWidget* parent)
        : QWidget(parent),
          _dateEdit(makeQChild<QDateEdit>(this)),
          _timeEdit(makeQChild<QTimeEdit>(this)),
          _todayButton(makeQChild<QPushButton>("Today", this))
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_dateEdit);
        layout->addWidget(_timeEdit);
        layout->addWidget(_todayButton);
        setLayout(layout);

        _dateEdit->setDisplayFormat("yyyy-MM-dd");
        _dateEdit->setCalendarPopup(true);

        _timeEdit->setDisplayFormat("HH:mm:ss");

        constexpr auto size = 48;
        _todayButton->setFixedWidth(size);

        _resetToToday();

        connect(
            _todayButton,
            &QPushButton::clicked,
            this,
            &TimestampField::_resetToToday
        );
    }

    /**
     * @brief get the transaction timestamp
     *
     * @return Timestamp
     */
    Timestamp TimestampField::getTimestamp() const
    {
        const QDateTime local{_dateEdit->date(), _timeEdit->time()};
        return Timestamp{local.toUTC()};
    }

    /**
     * @brief Reset the timestamp fields to the current date at 00:00:00
     */
    void TimestampField::_resetToToday()
    {
        _dateEdit->setDate(QDate::currentDate());
        _timeEdit->setTime(QTime(0, 0, 0));
    }

}   // namespace ui