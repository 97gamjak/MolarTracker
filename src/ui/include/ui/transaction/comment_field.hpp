#ifndef __UI__INCLUDE__UI__TRANSACTION__COMMENT_FIELD_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__COMMENT_FIELD_HPP__

#include <qstring.h>
#include <qwidget.h>

#include <optional>

class QLineEdit;   // Forward declaration

namespace ui
{

    /**
     * @brief Optional free-text comment field for a transaction
     *
     * Returns nullopt when the field is empty so callers can store
     * std::optional<std::string> directly without extra trimming logic.
     */
    class CommentField : public QWidget
    {
        Q_OBJECT

       private:
        /// the line edit widget for entering comments
        QLineEdit* _lineEdit;

       public:
        explicit CommentField(QWidget* parent = nullptr);

        [[nodiscard]] std::optional<std::string> getComment() const;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__COMMENT_FIELD_HPP__