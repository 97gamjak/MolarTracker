#include "ui/exceptions/exception_dialog.hpp"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "config/logging_base.hpp"
#include "logging/log_manager.hpp"
#include "logging/log_object.hpp"
#include "ui/exceptions/bug_report_dialog.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{
    /**
     * @brief Construct a new Exception Dialog:: Exception Dialog object
     *
     * @param title
     * @param details
     * @param parent
     */
    ExceptionDialog::ExceptionDialog(
        const QString& title,
        const QString& details,
        QWidget*       parent
    )
        : Dialog(parent)
    {
        setWindowTitle(title);
        setModal(true);

        auto* layout = utils::makeQChild<QVBoxLayout>(this);

        auto* summary = new QLabel(
            "The application encountered a fatal error and must close."
        );
        summary->setWordWrap(true);

        auto* detailBox = utils::makeQChild<QPlainTextEdit>(details);
        detailBox->setReadOnly(true);
        detailBox->setVisible(false);

        auto* toggleButton = new QPushButton("Show details");
        auto* copyButton   = new QPushButton("Copy");
        auto* reportButton = new QPushButton("Report Bug");
        auto* closeButton  = new QPushButton("Close");

        connect(
            toggleButton,
            &QPushButton::clicked,
            this,
            [this, toggleButton, detailBox]()
            {
                const bool visible = !detailBox->isVisible();
                detailBox->setVisible(visible);
                toggleButton->setText(
                    visible ? "Hide details" : "Show details"
                );
                this->adjustSize();
            }
        );

        connect(
            copyButton,
            &QPushButton::clicked,
            this,
            [details]() { QApplication::clipboard()->setText(details); }
        );

        connect(
            reportButton,
            &QPushButton::clicked,
            this,
            [this, details]()
            {
                BugReportDialog dlg(details, this);
                dlg.exec();
            }
        );

        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

        layout->addWidget(summary);
        layout->addWidget(toggleButton);
        layout->addWidget(detailBox);
        layout->addWidget(copyButton);
        layout->addWidget(reportButton);
        layout->addWidget(closeButton);
    }

    /**
     * @brief Show a fatal exception dialog
     *
     * @param title The title of the dialog
     * @param logObject The log object containing the details of the exception,
     * this will be logged as an error and shown in the details of the dialog
     * @param parent The parent widget of the dialog
     */
    void ExceptionDialog::showFatal(
        const QString&     title,
        logging::LogObject logObject,
        QWidget*           parent
    )
    {
        // we set the log level to error to make sure it is visible in the log
        // viewer and stands out from other log messages
        logObject.level = LogLevel::Error;

        logging::LogManager::getInstance().log(logObject);

        ExceptionDialog dlg(
            title,
            QString::fromStdString(logObject.message),
            parent
        );
        dlg.exec();
        // NOLINTNEXTLINE(concurrency-mt-unsafe)
        ::exit(0);
    }

    /**
     * @brief Show a fatal exception dialog without a parent widget
     *
     * @param title The title of the dialog
     * @param logObject The log object containing the details of the exception,
     * this will be logged as an error and shown in the details of the dialog
     */
    void ExceptionDialog::showFatal(
        const QString&     title,
        logging::LogObject logObject
    )
    {
        showFatal(title, std::move(logObject), nullptr);
    }

}   // namespace ui