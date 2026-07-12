#include "ui/exceptions/bug_report_dialog.hpp"

#include <QDesktopServices>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSysInfo>
#include <QUrl>
#include <QVBoxLayout>

#include "config/constants/github_constants.hpp"
#include "http/http_client.hpp"
#include "logging/log_manager.hpp"
#include "utils/qt_helpers.hpp"

namespace
{
    /// Maximum number of characters of the exception details embedded in the
    /// pre-filled report body, keeping the resulting GitHub URL a reasonable
    /// length
    constexpr qsizetype MAX_DETAILS_LENGTH = 3000;

    QString defaultTitle(const QString& details)
    {
        const auto firstLine = details.left(details.indexOf('\n'));
        return "Bug: " + firstLine.left(75);
    }

    QString defaultBody(const QString& details)
    {
        auto truncatedDetails = details.left(MAX_DETAILS_LENGTH);
        if (details.length() > MAX_DETAILS_LENGTH)
            truncatedDetails += "\n... [truncated]";

        const auto logFilePath = QString::fromStdString(
            logging::LogManager::getInstance().getCurrentLogFilePath()
        );

        return QString(
                   "### Environment\n"
                   "- App version: %1\n"
                   "- OS: %2\n\n"
                   "### Details\n"
                   "```\n%3\n```\n\n"
                   "_Full log file: %4_\n"
        )
            .arg(QString::fromStdString(GithubConstants::version))
            .arg(QSysInfo::prettyProductName())
            .arg(truncatedDetails)
            .arg(logFilePath);
    }
}   // namespace

namespace ui
{
    /**
     * @brief Construct a new Bug Report Dialog:: Bug Report Dialog object
     *
     * @param details The exception details (message and stack trace) to
     * pre-fill the bug report with
     * @param parent
     */
    BugReportDialog::BugReportDialog(const QString& details, QWidget* parent)
        : Dialog(parent)
    {
        setWindowTitle("Report Bug on GitHub");
        resize(600, 500);
        _buildUI(details);
    }

    /**
     * @brief Build the UI layout for the bug report dialog
     *
     * @param details The exception details used to pre-fill the report
     */
    void BugReportDialog::_buildUI(const QString& details)
    {
        auto* layout = utils::makeQChild<QVBoxLayout>(this);

        auto* infoLabel = utils::makeQChild<QLabel>(
            "Review and edit the report below, then open it on GitHub to "
            "submit it.",
            this
        );
        infoLabel->setWordWrap(true);

        _titleEdit = utils::makeQChild<QLineEdit>(defaultTitle(details), this);
        _bodyEdit =
            utils::makeQChild<QPlainTextEdit>(defaultBody(details), this);

        auto* openButton =
            utils::makeQChild<QPushButton>("Open in Browser", this);
        auto* cancelButton = utils::makeQChild<QPushButton>("Cancel", this);

        layout->addWidget(infoLabel);
        layout->addWidget(_titleEdit);
        layout->addWidget(_bodyEdit);
        layout->addWidget(openButton);
        layout->addWidget(cancelButton);

        connect(
            openButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                const auto encodedTitle = http::HttpClient::urlEncode(
                    _titleEdit->text().toStdString()
                );
                const auto encodedBody = http::HttpClient::urlEncode(
                    _bodyEdit->toPlainText().toStdString()
                );

                const auto url = GithubConstants::getGithubIssuesUrl() +
                                  "/new?title=" + encodedTitle +
                                  "&body=" + encodedBody;

                QDesktopServices::openUrl(QUrl{url.c_str()});
            }
        );
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }

}   // namespace ui
