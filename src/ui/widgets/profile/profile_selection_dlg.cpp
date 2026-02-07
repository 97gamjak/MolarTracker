#include "profile_selection_dlg.hpp"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "config/constants.hpp"
#include "utils/qt_helpers.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_profileSelectionDialog
#include "logging/log_macros.hpp"

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

        connect(
            _buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &ProfileSelectionDialog::_emitOk
        );
        connect(
            _buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &ProfileSelectionDialog::_emitCancel
        );

        // disable buttons until a profile is selected
        _buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        connect(
            _profileListWidget,
            &QListWidget::itemSelectionChanged,
            [this]()
            {
                // disable the OK button if no profile is selected, enable it
                // otherwise
                const bool hasSelection =
                    !_profileListWidget->selectedItems().empty();
                _buttonBox->button(QDialogButtonBox::Ok)
                    ->setEnabled(hasSelection);
            }
        );

        mainLayout->addWidget(_buttonBox);
    }

    void ProfileSelectionDialog::_emit(const Action& action)
    {
        const auto selectedItems = _profileListWidget->selectedItems();

        if (selectedItems.empty())
        {
            // this should never happen, as the OK button is disabled when no
            // profile is selected. But we check it just in case to prevent
            // crashes in case of unexpected issues.
            LOG_ERROR(
                "No profile selected, but OK button was clicked. This should "
                "never happen. If you see this message, please report it to "
                "the developers under " +
                Constants::getGithubIssuesUrl()
            );
            return;
        }

        const auto profileName = selectedItems.first()->text().toStdString();

        emit requested(action, profileName);
    }

    void ProfileSelectionDialog::_emitOk() { _emit(Action::Ok); }

    void ProfileSelectionDialog::_emitCancel() { _emit(Action::Cancel); }

}   // namespace ui