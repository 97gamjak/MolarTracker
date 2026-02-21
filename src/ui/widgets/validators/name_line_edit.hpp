#ifndef __UI__WIDGETS__VALIDATORS__NAME_LINE_EDIT_HPP__
#define __UI__WIDGETS__VALIDATORS__NAME_LINE_EDIT_HPP__

#include <QRegularExpression>

#include "line_validator.hpp"

namespace ui
{
    /**
     * @brief A LineValidator for validating names, this class implements the
     * _validateNonEmpty function to provide specific validation logic for
     * names, such as checking for invalid characters or reserved words. This
     * class can be used for validating profile names, service names, etc.
     */
    class NameLineEdit : public LineValidator
    {
        Q_OBJECT

       private:
        /// A regular expression for allowed characters in names, this allows
        /// letters, numbers, underscores, hyphens, and periods. This can be
        /// adjusted as needed to allow or disallow certain characters in names.
        inline static const QRegularExpression _allowedCharsRegex =
            QRegularExpression(R"(^[A-Za-z0-9_\-.]+$)");

       public:
        explicit NameLineEdit(QWidget* parent = nullptr);

       protected:
        [[nodiscard]] QString _getRequiredErrorMessage() const override;
        [[nodiscard]] std::pair<bool, QString> _validateNonEmpty(
            QStringView trimmedText
        ) const override;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__VALIDATORS__NAME_LINE_EDIT_HPP__