#include "add_profile_dlg.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
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
     * @param parent Parent widget
     */
    AddProfileDialog::AddProfileDialog(
        app::ProfileStore&  profileStore,
        settings::Settings& settings,
        UndoStack&          undoStack,
        QWidget*            parent
    )
        : QDialog{parent},
          _profileStore{profileStore},
          _settings{settings},
          _undoStack{undoStack}
    {
        setWindowTitle("Add New Profile");
        resize(400, 200);
        utils::moveDialogToParentScreenCenter(this, parent);

        _buildUI();
    }

    /**
     * @brief Accepts the dialog and adds the new profile
     *
     * @note Overrides QDialog::accept()
     */
    void AddProfileDialog::accept()
    {
        // TODO(97gamjak): wait for std::expected implementation in UndoStack
        // https://97gamjak.atlassian.net/browse/MOLTRACK-80
        // TODO(97gamjak): add error handling and display error message to user
        // https://97gamjak.atlassian.net/browse/MOLTRACK-73
        const auto result = _undoStack.makeAndDo<AddProfileCommand>(
            _profileStore,
            _settings,
            _getProfile(),
            _setActiveCheckBox->isChecked(),
            _setAsDefaultCheckBox->isChecked()
        );

        if (!result)
            return;

        QDialog::accept();
    }

    /**
     * @brief build the UI
     *
     */
    void AddProfileDialog::_buildUI()
    {
        QDialog::setModal(true);

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
        auto* formLayout = new QFormLayout{};
        _nameLineEdit    = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Name:"}, _nameLineEdit);

        _emailLineEdit = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Email:"}, _emailLineEdit);

        _mainLayout->addLayout(formLayout);
    }

    /**
     * @brief build the toggle section
     *
     */
    void AddProfileDialog::_buildToggleSection()
    {
        auto* toggleLayout = new QHBoxLayout{};

        _setActiveCheckBox = new QCheckBox{"Set as Active Profile", this};
        toggleLayout->addWidget(_setActiveCheckBox);

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
        auto* buttonLayout = new QHBoxLayout{};

        _addButton = new QPushButton{"Add Profile", this};
        buttonLayout->addWidget(_addButton);
        connect(_addButton, &QPushButton::clicked, this, &QDialog::accept);

        _cancelButton = new QPushButton{"Cancel", this};
        buttonLayout->addWidget(_cancelButton);
        connect(_cancelButton, &QPushButton::clicked, this, &QDialog::reject);

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
     * @brief get a profile draft from the line edits
     *
     * @return drafts::ProfileDraft
     */
    drafts::ProfileDraft AddProfileDialog::_getProfile() const
    {
        return drafts::ProfileDraft{
            _nameLineEdit->text().toStdString(),
            _emailLineEdit->text().toStdString()
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

}   // namespace ui