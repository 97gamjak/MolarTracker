#include "exception_dialog.hpp"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "config/logging_base.hpp"
#include "logging/log_manager.hpp"

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
        : QDialog(parent)
    {
        setWindowTitle(title);
        setModal(true);
        setMinimumWidth(600);

        auto* layout = new QVBoxLayout(this);

        auto* summary = new QLabel(
            "The application encountered a fatal error and must close."
        );
        summary->setWordWrap(true);

        auto* detailBox = new QPlainTextEdit(details);
        detailBox->setReadOnly(true);
        detailBox->setVisible(false);

        auto* toggleButton = new QPushButton("Show details");
        // TODO(97gamjak): make this a bug report button for GitHub later on
        // https://97gamjak.atlassian.net/browse/MOLTRACK-53
        auto* copyButton  = new QPushButton("Copy");
        auto* closeButton = new QPushButton("Close");

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

        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

        layout->addWidget(summary);
        layout->addWidget(toggleButton);
        layout->addWidget(detailBox);
        layout->addWidget(copyButton);
        layout->addWidget(closeButton);
    }

    /**
     * @brief Show a fatal exception dialog
     *
     * @param title The title of the dialog
     * @param logObject The log object containing the details of the exception,
     * this will be logged as an error and shown in the details of the dialog
     */
    void ExceptionDialog::showFatal(const QString& title, LogObject logObject)
    {
        // we set the log level to error to make sure it is visible in the log
        // viewer and stands out from other log messages
        logObject.level = LogLevel::Error;

        LogManager::getInstance().log(logObject);

        ExceptionDialog dlg(title, QString::fromStdString(logObject.message));
        dlg.exec();
        QCoreApplication::quit();
    }

}   // namespace ui