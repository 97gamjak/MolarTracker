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
     * @param onlyDateEdit Whether to only allow editing the date (true) or also
     * allow editing the time (false)
     * @param parent
     */
    TimestampField::TimestampField(bool onlyDateEdit, QWidget* parent)
        : QWidget(parent),
          _dateEdit(makeQChild<QDateEdit>(this)),
          _timeEdit(nullptr),
          _todayButton(makeQChild<QPushButton>("Today", this)),
          onlyDateEdit(onlyDateEdit)
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_dateEdit);
        layout->addWidget(_todayButton);
        setLayout(layout);

        _dateEdit->setDisplayFormat("yyyy-MM-dd");
        _dateEdit->setCalendarPopup(true);

        if (!onlyDateEdit)
        {
            _timeEdit = makeQChild<QTimeEdit>(this);
            _timeEdit->setDisplayFormat("HH:mm:ss");
            layout->addWidget(_timeEdit);
        }

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
        const QDateTime local{
            _dateEdit->date(),
            onlyDateEdit ? QTime(0, 0, 0) : _timeEdit->time()
        };
        return Timestamp{local.toUTC()};
    }

    /**
     * @brief Reset the timestamp fields to the current date at 00:00:00
     */
    void TimestampField::_resetToToday()
    {
        _dateEdit->setDate(QDate::currentDate());
        if (!onlyDateEdit)
            _timeEdit->setTime(QTime(0, 0, 0));
    }

}   // namespace ui