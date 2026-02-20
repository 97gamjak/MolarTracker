#include "add_profile_dlg.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/store/profile_store.hpp"
#include "drafts/profile_draft.hpp"
#include "settings/settings.hpp"
#include "ui/commands/add_profile_command.hpp"
#include "ui/commands/undo_stack.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Constructs a new Add Profile Dialog
     *
     * @param profileStore Reference to the Profile Store
     * @param settings Reference to the Settings
     * @param undoStack Reference to the Undo Stack
     * @param canBeClosed Whether the dialog can be closed without adding a
     * profile
     * @param parent Parent widget
     */
    AddProfileDialog::AddProfileDialog(
        app::ProfileStore&  profileStore,
        settings::Settings& settings,
        UndoStack&          undoStack,
        bool                canBeClosed,
        QWidget*            parent
    )
        : QDialog{parent},
          _profileStore{profileStore},
          _settings{settings},
          _undoStack{undoStack},
          _canBeClosed{canBeClosed}
    {
        setWindowTitle("Add New Profile");
        resize(400, 200);
        utils::moveDialogToParentScreenCenter(this, parent);

        _buildUI();
    }

    /**
     * @brief build the UI
     *
     */
    void AddProfileDialog::_buildUI()
    {
        QDialog::setModal(true);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _mainLayout = new QVBoxLayout{this};

        _buildFormSection();

        _buildToggleSection();

        _buildButtonSection();
    }

    /**
     * @brief build the form section for user input
     *
     */
    void AddProfileDialog::_buildFormSection()
    {
        // NOLINTBEGIN(cppcoreguidelines-owning-memory)
        auto* formLayout = new QFormLayout{};
        _nameLineEdit    = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Name:"}, _nameLineEdit);

        _emailLineEdit = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Email:"}, _emailLineEdit);
        // NOLINTEND(cppcoreguidelines-owning-memory)

        _mainLayout->addLayout(formLayout);
    }

    /**
     * @brief build the toggle section
     *
     */
    void AddProfileDialog::_buildToggleSection()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* toggleLayout = new QHBoxLayout{};

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _setActiveCheckBox = new QCheckBox{"Set as Active Profile", this};
        toggleLayout->addWidget(_setActiveCheckBox);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _setAsDefaultCheckBox = new QCheckBox{"Set as Default Profile", this};
        toggleLayout->addWidget(_setAsDefaultCheckBox);

        _setActiveCheckBox->setChecked(false);
        _setAsDefaultCheckBox->setChecked(false);

        _updateToggleStates();

        _mainLayout->addLayout(toggleLayout);
    }

    /**
     * @brief build the button section
     *
     */
    void AddProfileDialog::_buildButtonSection()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* buttonLayout = new QHBoxLayout{};

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _addButton = new QPushButton{"Add Profile", this};
        buttonLayout->addWidget(_addButton);
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &AddProfileDialog::_emitOk
        );

        if (_canBeClosed)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            _cancelButton = new QPushButton{"Cancel", this};
            buttonLayout->addWidget(_cancelButton);
            connect(
                _cancelButton,
                &QPushButton::clicked,
                this,
                &AddProfileDialog::_emitCancel
            );
        }

        _mainLayout->addLayout(buttonLayout);
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
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Profile Name Already Exists");
        msgBox.setText(
            "A profile with the same name already exists. Please choose a "
            "different name."
        );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
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

}   // namespace ui