#include "ui/profile/add_profile_dlg.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "app/store/profile_store.hpp"
#include "drafts/profile_draft.hpp"

namespace ui
{

    AddProfileDialog::AddProfileDialog(
        app::ProfileStore& profileStore,
        QWidget*           parent
    )
        : QDialog{parent}, _profileStore{profileStore}
    {
        setWindowTitle("Add New Profile");
        resize(400, 200);

        _buildUI();
    }

    void AddProfileDialog::accept()
    {
        // You can add validation logic here if needed
        const auto draft = getProfile();

        const auto result = _profileStore.addProfile(draft);

        if (result != app::ProfileStoreResult::Ok)
        {
            // Handle errors (e.g., show a message box) if needed
        }

        if (_setAsActive)
        {
            const auto _result = _profileStore.setActiveProfile(draft.name);
            if (_result != app::ProfileStoreResult::Ok)
            {
                // Handle error if needed
            }
            else
            {
                // add logging
            }
        }
        QDialog::accept();
    }

    void AddProfileDialog::_buildUI()
    {
        QDialog::setModal(true);

        auto* mainLayout = new QVBoxLayout{this};

        auto* formLayout = new QFormLayout{};
        _nameLineEdit    = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Name:"}, _nameLineEdit);

        _emailLineEdit = new QLineEdit{this};
        formLayout->addRow(new QLabel{"Email:"}, _emailLineEdit);

        mainLayout->addLayout(formLayout);

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

        mainLayout->addLayout(buttonLayout);
    }

    drafts::ProfileDraft AddProfileDialog::getProfile() const
    {
        return drafts::ProfileDraft{
            _nameLineEdit->text().toStdString(),
            _emailLineEdit->text().toStdString()
        };
    }

}   // namespace ui