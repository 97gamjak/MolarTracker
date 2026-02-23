#ifndef __UI__WIDGETS__VALIDATORS__LINE_VALIDATOR_HPP__
#define __UI__WIDGETS__VALIDATORS__LINE_VALIDATOR_HPP__

#include <QLineEdit>

class QLabel;   // Forward declaration

namespace ui
{
    /**
     * A QLineEdit with:
     * - local validation (implemented by derived classes)
     * - optional/required behavior
     * - controller-provided external error (e.g. "name taken")
     * - built-in visual feedback (red border + tooltip)
     *
     * Contract:
     * - If required == false and trimmed text is empty => valid
     * - If required == true and trimmed text is empty => invalid
     * - If non-empty => derived class validates
     * - If external error is set => invalid regardless of local validation
     */
    class LineValidator : public QLineEdit
    {
        Q_OBJECT

       private:
        /// Flag indicating whether the current text is valid according to local
        /// validation and external error state
        bool _isValid{false};

        /// Flag indicating whether the field is required, if true, empty (or
        /// whitespace-only) input will be considered invalid
        bool _required{false};

        /// The error message to be shown in the tooltip when the input is
        /// invalid, this can be set by local validation or by an external error
        QString _errorText;

        /// Flag indicating whether there is an external error that should be
        /// shown
        bool _hasExternalError{false};
        /// The external error message to be shown in the tooltip if
        /// _hasExternalError is true
        QString _externalError;

        /// Optional label to show error messages below the line edit, this can
        /// be used if we want to show error messages persistently instead of
        /// only in the tooltip. If this is nullptr, error messages will only be
        /// shown in the tooltip.
        QLabel* _errorLabel = nullptr;

       public:
        explicit LineValidator(QWidget* parent);

        [[nodiscard]] bool    isValid() const;
        [[nodiscard]] QString getErrorText() const;

        void setRequired(bool required) noexcept;

        void setExternalError(QString message);
        void clearExternalError();

        void attachErrorLabel(QLabel* label);

       signals:
        /// Signal emitted when the validity of the input changes
        void validityChanged(bool isValid);
        /// Signal emitted when the trimmed text changes
        void valueChanged(const QString& trimmedValue);

       private slots:
        void onTextChanged(const QString&);

       protected:
        [[nodiscard]] virtual QString _getRequiredErrorMessage() const;

        /**
         * @brief a pure virtual function that derived classes must implement to
         * provide specific validation logic for the input. This function will
         * only be called if the trimmed text is non-empty, and it should return
         * a pair of (isValid, errorMessage). If isValid is true, errorMessage
         * will be ignored. If isValid is false, errorMessage will be shown in
         * the tooltip as the reason for invalidity.
         *
         * @param trimmedText
         * @return std::pair<bool, QString>
         */
        [[nodiscard]] virtual std::pair<bool, QString> _validateNonEmpty(
            QStringView trimmedText
        ) const = 0;

       private:
        void _recompute();
        void _applyFeedback(bool isValid, const QString& errorMessage);
        [[nodiscard]] std::pair<bool, QString> _validate(
            QStringView rawText
        ) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__VALIDATORS__LINE_VALIDATOR_HPP__