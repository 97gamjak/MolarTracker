#include "line_validator.hpp"

#include <QLabel>

namespace ui
{
    /**
     * @brief Construct a new Line Validator:: Line Validator object
     *
     * @param parent Parent widget
     */
    LineValidator::LineValidator(QWidget* parent) : QLineEdit(parent)
    {
        connect(
            this,
            &QLineEdit::textChanged,
            this,
            &LineValidator::onTextChanged
        );
        _recompute();
    }

    /**
     * @brief Check if the current text in the line edit is valid according to
     * local validation and external error state. This will return true if the
     * input is considered valid, and false if it is invalid. The validity is
     * determined by the combination of local validation logic (implemented by
     * derived classes) and any external error that may be set.
     *
     * @return bool true if the current input is valid, false otherwise
     */
    bool LineValidator::isValid() const { return _isValid; }

    /**
     * @brief Get the error message to be shown in the tooltip when the input is
     * invalid, this can be set by local validation or by an external error. If
     * the input is valid, this will return an empty string.
     *
     * @return QString the error message to show in the tooltip when the input
     * is invalid, or an empty string if the input is valid
     */
    QString LineValidator::getErrorText() const { return _errorText; }

    /**
     * @brief Set whether the field is required or not, this will affect the
     * validation logic such that if the field is required, empty (or
     * whitespace-only) input will be considered invalid. If the required state
     * changes, it will trigger re-validation of the current text to update the
     * validity state accordingly.
     *
     * @param required whether the field should be considered required or not
     */
    void LineValidator::setRequired(bool required) noexcept
    {
        if (_required != required)
        {
            _required = required;
            _recompute();
        }
    }

    /**
     * @brief set an external error message, this will cause the validator to
     * consider the input invalid and show the given error message in the
     * tooltip. This can be used to indicate validation errors that are not
     * related to the local validation logic, for example errors from
     * asynchronous operations or from other parts of the application. When an
     * external error is set, it takes precedence over local validation results
     * until it is cleared.
     *
     * @param message the external error message to show in the tooltip
     */
    void LineValidator::setExternalError(QString message)
    {
        _hasExternalError = true;
        _externalError    = std::move(message);
        _recompute();
    }

    /**
     * @brief clear any external error that is currently set, this will cause
     * the validator to re-validate the current text and update the validity
     * state accordingly. This should be called when the external error
     * condition has been resolved, for example after an asynchronous validation
     * completes or when the user modifies the input after an external error was
     * shown.
     *
     */
    void LineValidator::clearExternalError()
    {
        if (_hasExternalError)
        {
            _hasExternalError = false;
            _externalError.clear();
            _recompute();
        }
    }

    /**
     * @brief attach a QLabel to show error messages below the line edit, this
     * can be used if we want to show error messages persistently instead of
     * only in the tooltip. If this is not called, error messages will only be
     * shown in the tooltip. If a label is attached, it will be updated with the
     * current error message and shown/hidden based on the validity state.
     *
     * @param label the QLabel to attach for showing error messages below the
     * line edit
     */
    void LineValidator::attachErrorLabel(QLabel* label)
    {
        const auto visible = (_errorLabel && _errorLabel->isVisible());
        const auto msg     = (_errorLabel) ? _errorLabel->text() : QString{};

        _errorLabel = label;

        if (_errorLabel)
        {
            _errorLabel->setStyleSheet(
                "QLabel { color: #c00; font-size: 11px; }"
            );
            _errorLabel->setVisible(visible);
            _errorLabel->setText(msg);
        }
    }

    /**
     * @brief Slot to handle text changes in the line edit, this will trigger
     * re-validation of the input and emit valueChanged with the trimmed text.
     * If there is an external error, it will be cleared when the user changes
     * the text.
     *
     */
    void LineValidator::onTextChanged(const QString&)
    {
        if (_hasExternalError)
        {
            _hasExternalError = false;
            _externalError.clear();
        }

        _recompute();
        emit valueChanged(text().trimmed());
    }

    /**
     * @brief get the error message to be shown when the field is required but
     * empty
     *
     * @return QString
     */
    QString LineValidator::_getRequiredErrorMessage() const
    {
        return "This field is required.";
    }

    /**
     * @brief validate the given text, returns a pair of (isValid,
     * errorMessage). If isValid is true, errorMessage will be ignored. If
     * isValid is false, errorMessage will be shown in the tooltip.
     *
     * @param rawText the text to validate; it may be empty or contain
     * surrounding whitespace and will be trimmed before validation
     * @return std::pair<bool, QString>
     */
    std::pair<bool, QString> LineValidator::_validate(QStringView rawText) const
    {
        const auto t = rawText.trimmed();

        if (t.isEmpty())
        {
            if (_required)
                return {false, _getRequiredErrorMessage()};

            // optional and empty => ok
            return {true, {}};
        }

        return _validateNonEmpty(QStringView{t});
    }

    /**
     * @brief recompute the validity state based on the current text and
     * external error, this will call the local validation logic and combine it
     * with any external error to determine the overall validity of the input.
     * It will then apply visual feedback and emit validityChanged if the
     * validity state has changed. This should be called whenever the text
     * changes or when an external error is set/cleared to update the validity
     * state accordingly.
     *
     */
    void LineValidator::_recompute()
    {
        bool ok = false;

        if (_hasExternalError)
        {
            ok         = false;
            _errorText = _externalError;
        }
        else
        {
            const auto local = _validate(text());
            ok               = local.first;
            _errorText       = local.second;
        }

        const bool changed = (ok != _isValid);
        _isValid           = ok;

        _applyFeedback(ok, _errorText);

        if (changed)
            emit validityChanged(_isValid);
    }

    /**
     * @brief apply visual feedback for the current validity state, this is
     * called whenever the validity state changes to update the UI accordingly
     *
     * @param isValid whether the current text is valid or not
     * @param errorMessage the error message to show in the tooltip if the input
     * is invalid, ignored if isValid is true
     */
    void LineValidator::_applyFeedback(
        bool           isValid,
        const QString& errorMessage
    )
    {
        if (isValid)
        {
            setStyleSheet({});
            setToolTip({});
            if (_errorLabel)
            {
                _errorLabel->clear();
                _errorLabel->setVisible(false);
            }
            return;
        }

        setStyleSheet("QLineEdit { border: 1px solid #c00; }");
        setToolTip(errorMessage);

        if (_errorLabel)
        {
            _errorLabel->setText(errorMessage);
            _errorLabel->setVisible(true);
        }
    }

}   // namespace ui