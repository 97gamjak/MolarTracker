#include "ui/update/update_available_dialog.hpp"

#include <QCheckBox>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct an UpdateAvailableDialog.
     *
     * @param latestVersion The version that is available for download.
     * @param parent        Optional parent widget.
     */
    UpdateAvailableDialog::UpdateAvailableDialog(
        const utils::SemVer& latestVersion,
        QWidget*             parent
    )
        : Dialog{parent}
    {
        setWindowTitle("Update Available");

        auto* layout = utils::makeQChild<QVBoxLayout>(this);

        auto* versionLabel = utils::makeQChild<QLabel>(
            QString{"A new version of MolarTracker is available: <b>%1</b>"}
                .arg(QString::fromStdString(latestVersion.toString())),
            this
        );
        versionLabel->setTextFormat(Qt::RichText);
        layout->addWidget(versionLabel);

        auto* releasesButton =
            utils::makeQChild<QPushButton>("Open Releases Page", this);
        connect(
            releasesButton,
            &QPushButton::clicked,
            this,
            []()
            {
                QDesktopServices::openUrl(
                    QUrl{"https://github.com/97gamjak/MolarTracker/releases"}
                );
            }
        );
        layout->addWidget(releasesButton);

        _dismissCheckBox = utils::makeQChild<QCheckBox>(
            "Don't show again for this version",
            this
        );
        layout->addWidget(_dismissCheckBox);

        auto* buttons =
            utils::makeQChild<QDialogButtonBox>(QDialogButtonBox::Ok, this);
        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        layout->addWidget(buttons);
    }

    /**
     * @brief Returns whether the user checked the "don't show again" box.
     *
     * @return true if the checkbox is checked
     */
    bool UpdateAvailableDialog::isDismissedForVersion() const
    {
        return _dismissCheckBox->isChecked();
    }

}   // namespace ui
