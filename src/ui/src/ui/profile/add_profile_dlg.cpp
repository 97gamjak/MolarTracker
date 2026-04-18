#include "ui/profile/add_profile_dlg.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "drafts/profile_draft.hpp"
#include "ui/validators/validators.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Constructs a new Add Profile Dialog
     *
     * @param settings Reference to the Settings
     * @param canBeClosed Whether the dialog can be closed without adding a
     * profile
     * @param parent Parent widget
     */
    AddProfileDialog::AddProfileDialog(
        std::shared_ptr<Settings> settings,
        bool                      canBeClosed,
        QWidget*                  parent
    )
        : Dialog{parent},
          _settings{std::move(settings)},
          _canBeClosed{canBeClosed}
    {
        setWindowTitle("Add New Profile");
        resize(_settings->dialogSize.first, _settings->dialogSize.second);
        utils::moveDialogToParentScreenCenter(this, parent);

        _buildUI();
    }

    /**
     * @brief Construct a new Add Profile Dialog:: Add Profile Dialog object
     *
     * @param settings
     * @param parent
     */
    AddProfileDialog::AddProfileDialog(
        std::shared_ptr<Settings> settings,
        QWidget*                  parent
    )
        : AddProfileDialog(
              std::move(settings),
              true,   // true to allow closing without adding a profile
              parent
          )
    {
    }

    /**
     * @brief build the UI
     *
     */
    void AddProfileDialog::_buildUI()
    {
        QDialog::setModal(true);

        auto* mainLayout = utils::makeQChild<QVBoxLayout>(this);

        _buildFormSection(mainLayout);

        _buildToggleSection(mainLayout);

        _buildButtonSection(mainLayout);
    }

    /**
     * @brief build the form section for user input
     *
     * @param parent The parent layout of the form section
     */
    void AddProfileDialog::_buildFormSection(QVBoxLayout* parent)
    {
        auto* formLayout = utils::makeQChild<QFormLayout>();

        auto [nameLineEdit, nameContainer] =
            createLineEditWithLabel<NameLineEdit>(this);
        nameLineEdit->setRequired(true);

        _nameLineEdit = nameLineEdit;
        formLayout->addRow(utils::makeQChild<QLabel>("Name*:"), nameContainer);

        auto* emailErrorLabel = utils::makeQChild<QLabel>();

        _emailLineEdit = utils::makeQChild<EmailLineEdit>(this);
        _emailLineEdit->attachErrorLabel(emailErrorLabel);

        auto* emailContainer = utils::makeQChild<QWidget>();
        auto* emailLayout    = utils::makeQChild<QVBoxLayout>(emailContainer);
        emailLayout->setContentsMargins(0, 0, 0, 0);
        emailLayout->setSpacing(2);
        emailLayout->addWidget(_emailLineEdit);
        emailLayout->addWidget(emailErrorLabel);

        formLayout->addRow(utils::makeQChild<QLabel>("Email:"), emailContainer);

        parent->addLayout(formLayout);
    }

    /**
     * @brief build the toggle section
     *
     * @param parent The parent layout of the toggle section
     */
    void AddProfileDialog::_buildToggleSection(QVBoxLayout* parent)
    {
        auto* toggleLayout = utils::makeQChild<QHBoxLayout>();

        _setActiveCheckBox =
            utils::makeQChild<QCheckBox>("Set as Active Profile");
        toggleLayout->addWidget(_setActiveCheckBox);

        _setAsDefaultCheckBox =
            utils::makeQChild<QCheckBox>("Set as Default Profile");
        toggleLayout->addWidget(_setAsDefaultCheckBox);

        _setActiveCheckBox->setChecked(false);
        _setAsDefaultCheckBox->setChecked(false);

        _updateToggleStates();

        parent->addLayout(toggleLayout);
    }

    /**
     * @brief build the button section
     *
     * @param parent The parent layout of the button section
     */
    void AddProfileDialog::_buildButtonSection(QVBoxLayout* parent)
    {
        auto* buttonLayout = utils::makeQChild<QHBoxLayout>();

        _addButton = utils::makeQChild<QPushButton>("Add Profile");

        // check the validity of the input to enable or disable the add button
        _addButton->setEnabled(false);
        connect(
            _nameLineEdit,
            &NameLineEdit::validityChanged,
            _addButton,
            &QPushButton::setEnabled
        );

        // connect the add button to emit the Ok action with the profile draft
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &AddProfileDialog::_emitOk
        );

        buttonLayout->addWidget(_addButton);

        if (_canBeClosed)
        {
            _cancelButton = utils::makeQChild<QPushButton>("Cancel");
            buttonLayout->addWidget(_cancelButton);
            connect(
                _cancelButton,
                &QPushButton::clicked,
                this,
                &AddProfileDialog::_emitCancel
            );
        }

        parent->addLayout(buttonLayout);
    }

    /**
     * @brief set the enforce default flag
     *
     * @note setting this flag needs a subsequent update of the toggle states
     *
     * @param value
     */
    void AddProfileDialog::setEnforceDefaultProfile(bool value)
    {
        _enforceDefaultProfile = value;

        _updateToggleStates();
    }

    /**
     * @brief check if the active checkbox is checked
     *
     * @return true if checked, false otherwise
     */
    bool AddProfileDialog::isActiveChecked() const
    {
        return _setActiveCheckBox->isChecked();
    }

    /**
     * @brief check if the default checkbox is checked
     *
     * @return true if checked, false otherwise
     */
    bool AddProfileDialog::isDefaultChecked() const
    {
        return _setAsDefaultCheckBox->isChecked();
    }

    /**
     * @brief show an error message if the profile name already exists
     *
     */
    void AddProfileDialog::showNameAlreadyExistsError()
    {
        _nameLineEdit->setExternalError(
            "A profile with the same name already exists."
        );
    }

    /**
     * @brief get a profile draft from the line edits
     *
     * @return drafts::ProfileDraft
     */
    drafts::ProfileDraft AddProfileDialog::_getProfile() const
    {
        return drafts::ProfileDraft{
            .name  = _nameLineEdit->text().toStdString(),
            .email = _emailLineEdit->text().toStdString()
        };
    }

    /**
     * @brief update the toggle states, if enforceDefaultProfile
     * is true set active and set default will be disabled
     *
     */
    void AddProfileDialog::_updateToggleStates()
    {
        if (!_enforceDefaultProfile)
        {
            _setActiveCheckBox->setEnabled(true);
            _setAsDefaultCheckBox->setEnabled(true);
        }
        else
        {
            _setActiveCheckBox->setChecked(true);
            _setActiveCheckBox->setEnabled(false);
            _setAsDefaultCheckBox->setChecked(true);
            _setAsDefaultCheckBox->setEnabled(false);
        }
    }

    /**
     * @brief emit the requested signal with the given action and profile draft
     *
     * @param action
     */
    void AddProfileDialog::_emit(const Action& action)
    {
        emit requested(action, _getProfile());
    }

    /**
     * @brief emit the requested signal with the Ok action and profile draft
     *
     */
    void AddProfileDialog::_emitOk() { _emit(Action::Ok); }

    /**
     * @brief emit the requested signal with the Cancel action
     *
     */
    void AddProfileDialog::_emitCancel() { _emit(Action::Cancel); }

    /**
     * @brief Handle the close event of the dialog. If the dialog is closed
     * without adding a profile, emit the cancel action.
     *
     */
    void AddProfileDialog::reject()
    {
        if (_canBeClosed)
        {
            _emitCancel();
            QDialog::reject();
        }
    }

    /**
     * @brief Construct a new Add Profile Dialog:: Settings:: Settings object
     *
     * @param _dialogSize The size of the dialog (width, height)
     */
    AddProfileDialog::Settings::Settings(std::pair<int, int> _dialogSize)
        : dialogSize(_dialogSize)
    {
    }

}   // namespace ui