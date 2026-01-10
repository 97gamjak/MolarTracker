#include "ui/profile/add_profile_dlg.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "drafts/profile_draft.hpp"

namespace ui
{

    AddProfileDialog::AddProfileDialog(QWidget* parent) : QDialog{parent}
    {
        setWindowTitle("Add New Profile");
        resize(400, 200);

        _buildUI();
    }

    void AddProfileDialog::_buildUI()
    {
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