#include "profile_selection_dlg.hpp"

#include <QDebug>
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
     * @param canBeClosed Whether the dialog can be closed without selecting a
     * profile
     */
    ProfileSelectionDialog::ProfileSelectionDialog(
        QWidget*                        parent,
        const std::vector<std::string>& profiles,
        bool                            canBeClosed
    )
        : QDialog{parent}, _canBeClosed{canBeClosed}
    {
        setWindowTitle("Select Profile");
        utils::moveDialogToParentScreenCenter(this, parent);

        _buildUI(profiles);
    }

    /**
     * @brief Construct a new Profile Selection Dialog:: Profile Selection
     * Dialog object
     *
     * @param parent
     * @param profiles
     */
    ProfileSelectionDialog::ProfileSelectionDialog(
        QWidget*                        parent,
        const std::vector<std::string>& profiles
    )
        : ProfileSelectionDialog(parent, profiles, true)
    {
    }

    /**
     * @brief Builds the UI for the Profile Selection Dialog
     *
     * @param profiles List of profile names to display
     */
    void ProfileSelectionDialog::_buildUI(
        const std::vector<std::string>& profiles
    )
    {
        // NOLINTBEGIN(cppcoreguidelines-owning-memory)
        auto* mainLayout   = new QVBoxLayout{this};
        _profileListWidget = new QListWidget{this};
        // NOLINTEND(cppcoreguidelines-owning-memory)

        for (const auto& profileName : profiles)
            _profileListWidget->addItem(QString::fromStdString(profileName));

        mainLayout->addWidget(_profileListWidget);

        auto buttons = _canBeClosed
                           ? QDialogButtonBox::Ok | QDialogButtonBox::Cancel
                           : QDialogButtonBox::Ok;

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _buttonBox = new QDialogButtonBox(buttons, Qt::Horizontal, this);

        connect(
            _buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &ProfileSelectionDialog::_emitOk
        );

        // If the dialog can be closed, connect the rejected signal to emit the
        // cancel action. Otherwise, we ignore the rejected signal to prevent
        // the user from closing the dialog without selecting a profile.
        if (_canBeClosed)
        {
            connect(
                _buttonBox,
                &QDialogButtonBox::rejected,
                this,
                &ProfileSelectionDialog::_emitCancel
            );
        }

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

    /**
     * @brief Handle the close event of the dialog. If the dialog is closed
     * without selecting a profile, emit the cancel action.
     *
     */
    void ProfileSelectionDialog::reject()
    {
        if (_canBeClosed)
        {
            _emitCancel();
            QDialog::reject();
        }
    }

    /**
     * @brief Emit the requested signal with the given action and profile name
     *
     * @param action The action performed by the user (Ok or Cancel)
     * @param profileName The name of the selected profile (only valid if action
     * is Ok)
     */
    void ProfileSelectionDialog::_emit(
        const Action&      action,
        const std::string& profileName
    )
    {
        emit requested(action, profileName);
    }

    /**
     * @brief Emit the requested signal with the given action and an empty
     * profile name
     *
     * @param action The action performed by the user (Ok or Cancel)
     */
    void ProfileSelectionDialog::_emit(const Action& action)
    {
        emit requested(action, "");
    }

    /**
     * @brief Emit the requested signal with the Ok action and the selected
     * profile name
     *
     */
    void ProfileSelectionDialog::_emitOk()
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

        _emit(Action::Ok, profileName);
        accept();
    }

    /**
     * @brief Emit the requested signal with the Cancel action
     *
     */
    void ProfileSelectionDialog::_emitCancel() { _emit(Action::Cancel); }

}   // namespace ui