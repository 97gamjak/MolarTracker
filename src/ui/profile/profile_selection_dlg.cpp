#include "ui/profile/profile_selection_dlg.hpp"

#include <QVBoxLayout>

namespace ui
{

    ProfileSelectionDialog::ProfileSelectionDialog(
        QWidget*                 parent,
        std::vector<std::string> profiles
    )
        : QDialog{parent}
    {
        setWindowTitle("Select Profile");
        resize(400, 300);

        _buildUI(profiles);
    }

    void ProfileSelectionDialog::_buildUI(std::vector<std::string> profiles)
    {
        auto* mainLayout = new QVBoxLayout{this};

        _profileListWidget = new QListWidget{this};
        for (const auto& profileName : profiles)
        {
            _profileListWidget->addItem(QString::fromStdString(profileName));
        }
        mainLayout->addWidget(_profileListWidget);
    }

}   // namespace ui