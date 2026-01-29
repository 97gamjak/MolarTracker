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

namespace ui
{

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

        _buildUI();
    }

    void AddProfileDialog::accept()
    {
        const auto result = _undoStack.makeAndDo<AddProfileCommand>(
            _profileStore,
            _settings,
            getProfile(),
            _setActiveCheckBox->isChecked(),
            _setAsDefaultCheckBox->isChecked()
        );

        if (!result)
            return;

        QDialog::accept();
    }

    void AddProfileDialog::_buildUI()
    {
        QDialog::setModal(true);

        _mainLayout = new QVBoxLayout{this};

        _buildFormSection();

        _buildToggleSection();

        _buildButtonSection();
    }

    void AddProfileDialog::_buildFormSection()
    {
        auto* formLayout = new QFormLayout{};
        _nameLineEdit    = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Name:"}, _nameLineEdit);

        _emailLineEdit = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Email:"}, _emailLineEdit);

        _mainLayout->addLayout(formLayout);
    }

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

    void AddProfileDialog::_buildButtonSection()
    {
        auto* buttonLayout = new QHBoxLayout{};

        _addProfileButton = new QPushButton{"Add Profile", this};
        buttonLayout->addWidget(_addProfileButton);
        connect(
            _addProfileButton,
            &QPushButton::clicked,
            this,
            &QDialog::accept
        );

        _cancelButton = new QPushButton{"Cancel", this};
        buttonLayout->addWidget(_cancelButton);
        connect(_cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        _mainLayout->addLayout(buttonLayout);
    }

    void AddProfileDialog::setEnforceDefaultProfile(bool value)
    {
        _enforceDefaultProfile = value;

        _updateToggleStates();
    }

    drafts::ProfileDraft AddProfileDialog::getProfile() const
    {
        return drafts::ProfileDraft{
            _nameLineEdit->text().toStdString(),
            _emailLineEdit->text().toStdString()
        };
    }

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