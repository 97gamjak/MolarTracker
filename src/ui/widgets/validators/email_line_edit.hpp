#ifndef __UI__WIDGETS__VALIDATORS__EMAIL_LINE_EDIT_HPP__
#define __UI__WIDGETS__VALIDATORS__EMAIL_LINE_EDIT_HPP__

#include "line_validator.hpp"

namespace ui
{
    /**
     * @brief A LineValidator for validating email addresses, this class
     * implements the _validateNonEmpty function to provide specific validation logic
     * for email addresses, such as checking for the presence of an "@" symbol
     * and ensuring that the email address is in a valid format. This class can
     * be used for validating email input fields in the application.
     */
    class EmailLineEdit : public LineValidator
    {
        Q_OBJECT

       public:
        explicit EmailLineEdit(QWidget* parent = nullptr);

       protected:
        [[nodiscard]] QString _getRequiredErrorMessage() const override;
        [[nodiscard]] std::pair<bool, QString> _validateNonEmpty(
            QStringView trimmedText
        ) const override;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__VALIDATORS__EMAIL_LINE_EDIT_HPP__