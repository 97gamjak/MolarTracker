#include "profile_selection_dlg.hpp"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QVBoxLayout>

#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Constructs a new Profile Selection Dialog
     *
     * @param parent Parent widget
     * @param profiles List of profile names to display
     */
    ProfileSelectionDialog::ProfileSelectionDialog(
        QWidget*                 parent,
        std::vector<std::string> profiles
    )
        : QDialog{parent}
    {
        setWindowTitle("Select Profile");
        resize(400, 300);
        utils::moveDialogToParentScreenCenter(this, parent);

        _buildUI(profiles);
    }

    /**
     * @brief Builds the UI for the Profile Selection Dialog
     *
     * @param profiles List of profile names to display
     */
    void ProfileSelectionDialog::_buildUI(std::vector<std::string> profiles)
    {
        auto* mainLayout   = new QVBoxLayout{this};
        _profileListWidget = new QListWidget{this};

        for (const auto& profileName : profiles)
            _profileListWidget->addItem(QString::fromStdString(profileName));

        mainLayout->addWidget(_profileListWidget);

        using enum QDialogButtonBox::StandardButton;
        _buttonBox = new QDialogButtonBox{Ok | Cancel, this};

        // TODO(97gamjak): make these buttons actually do something useful
        // https://97gamjak.atlassian.net/browse/MOLTRACK-77

        mainLayout->addWidget(_buttonBox);
    }

}   // namespace ui