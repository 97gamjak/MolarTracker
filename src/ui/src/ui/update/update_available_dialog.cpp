#include "ui/update/update_available_dialog.hpp"

#include <QCheckBox>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

#include "common/qt_helpers.hpp"
#include "common/version.hpp"
#include "config/constants/github_constants.hpp"

namespace ui
{

    /**
     * @brief Construct an UpdateAvailableDialog.
     *
     * @param latestVersion The version that is available for download.
     * @param parent        Optional parent widget.
     */
    UpdateAvailableDialog::UpdateAvailableDialog(
        const common::SemVer& latestVersion,
        QWidget*              parent
    )
        : Dialog{parent}
    {
        setWindowTitle("Update Available");

        auto* layout = common::makeQChild<QVBoxLayout>(this);

        auto* versionLabel = common::makeQChild<QLabel>(
            QString{"A new version of MolarTracker is available: <b>%1</b>"}
                .arg(latestVersion.toQString()),
            this
        );
        versionLabel->setTextFormat(Qt::RichText);
        layout->addWidget(versionLabel);

        auto* releasesButton =
            common::makeQChild<QPushButton>("Open Releases Page", this);

        connect(
            releasesButton,
            &QPushButton::clicked,
            this,
            []()
            {
                const auto url = GithubConstants::getGithubReleaseUrl();
                QDesktopServices::openUrl(QUrl{url.c_str()});
            }
        );
        layout->addWidget(releasesButton);

        auto* downloadButton =
            common::makeQChild<QPushButton>("Download Update", this);

        connect(
            downloadButton,
            &QPushButton::clicked,
            this,
            &UpdateAvailableDialog::downloadRequested
        );
        layout->addWidget(downloadButton);

        _dismissCheckBox = common::makeQChild<QCheckBox>(
            "Don't show again for this version",
            this
        );
        layout->addWidget(_dismissCheckBox);

        auto* buttons =
            common::makeQChild<QDialogButtonBox>(QDialogButtonBox::Ok, this);

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
