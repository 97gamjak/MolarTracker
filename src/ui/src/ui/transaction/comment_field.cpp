#include "ui/transaction/comment_field.hpp"

#include <qboxlayout.h>
#include <qlineedit.h>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Comment Field:: Comment Field object
     *
     * @param parent
     */
    CommentField::CommentField(QWidget* parent)
        : QWidget(parent), _lineEdit(makeQChild<QLineEdit>(this))
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_lineEdit);
        setLayout(layout);

        _lineEdit->setPlaceholderText("Optional comment...");
    }

    /**
     * @brief Get the comment text from the line edit
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> CommentField::getComment() const
    {
        const auto text = _lineEdit->text().trimmed();
        return text.isEmpty() ? std::nullopt
                              : std::optional{text.toStdString()};
    }

}   // namespace ui