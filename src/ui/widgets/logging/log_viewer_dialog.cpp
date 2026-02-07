#include "log_viewer_dialog.hpp"

#include <QCheckBox>
#include <QFile>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <filesystem>

#include "logging/log_manager.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_logging
#include "logging/log_macros.hpp"

namespace ui
{

    /**
     * @brief Construct a new Log Viewer Dialog:: Log Viewer Dialog object
     *
     * @param parent
     */
    LogViewerDialog::LogViewerDialog(QWidget* parent)
        : QDialog(parent),
          _textEdit(new QPlainTextEdit(this)),
          _reloadButton(new QPushButton(tr("Reload"), this)),
          _autoReloadCheckBox(new QCheckBox(tr("Auto Reload"), this)),
          _reloadTimer(new QTimer(this))
    {
        setWindowTitle(tr("Application Log"));
        resize(900, 600);

        _textEdit->setReadOnly(true);
        // TODO(97gamjak): maybe this should be configurable
        // https://97gamjak.atlassian.net/browse/MOLTRACK-103
        _textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        _textEdit->setMaximumBlockCount(50000);

        _autoReloadCheckBox->setChecked(false);
        // TODO(97gamjak): make this changeable via config and dialog
        // https://97gamjak.atlassian.net/browse/MOLTRACK-104
        _reloadTimer->setInterval(1000);   // 1000 ms

        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(_reloadButton);
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(_autoReloadCheckBox);

        auto* layout = new QVBoxLayout(this);
        layout->addWidget(_textEdit);
        layout->addLayout(buttonLayout);

        connect(
            _reloadButton,
            &QPushButton::clicked,
            this,
            &LogViewerDialog::reloadLog
        );

        connect(
            _autoReloadCheckBox,
            &QCheckBox::toggled,
            this,
            [this](bool isAutoReloadEnabled)
            {
                if (isAutoReloadEnabled)
                {
                    _reloadButton->setEnabled(false);
                    _reloadTimer->start();
                }
                else
                {
                    _reloadButton->setEnabled(true);
                    _reloadTimer->stop();
                }
            }
        );

        connect(
            _reloadTimer,
            &QTimer::timeout,
            this,
            &LogViewerDialog::reloadLog
        );

        _loadLogFile();
    }

    /**
     * @brief Load the log file content into the text edit.
     *
     */
    void LogViewerDialog::_loadLogFile()
    {
        auto&       logManager  = LogManager::getInstance();
        const auto& logFilePath = logManager.getCurrentLogFilePath();
        const auto  absPath     = std::filesystem::absolute(logFilePath);
        const auto  qStrPath    = QString::fromStdString(absPath.string());

        // we need to flush here to ensure all logs are written before reading
        // TODO(97gamjak): think of a clever flushing strategy to avoid
        // performance issues especially when auto-reload is enabled
        // https://97gamjak.atlassian.net/browse/MOLTRACK-105
        logManager.flush();

        QFile file(qStrPath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            _textEdit->setPlainText(
                tr("Failed to open log file:\n%1").arg(qStrPath)
            );
            LOG_ERROR("Failed to open log file: " + qStrPath.toStdString());
            return;
        }

        QTextStream stream(&file);
        _textEdit->setPlainText(stream.readAll());

        // Scroll to bottom
        // TODO(97gamjak): think of a better way to handle this
        // maybe only scroll to bottom if user did not scroll up manually
        // https://97gamjak.atlassian.net/browse/MOLTRACK-106
        auto cursor = _textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        _textEdit->setTextCursor(cursor);
    }

    /**
     * @brief Reload the log file content.
     *
     */
    void LogViewerDialog::reloadLog() { _loadLogFile(); }

    /**
     * @brief Handle the hide event.
     *
     * @param event
     */
    void LogViewerDialog::hideEvent(QHideEvent* event)
    {
        QDialog::hideEvent(event);
        _reloadTimer->stop();
    }

    /**
     * @brief Handle the show event.
     *
     * @param event
     */
    void LogViewerDialog::showEvent(QShowEvent* event)
    {
        QDialog::showEvent(event);

        reloadLog();
        if (_autoReloadCheckBox->isChecked())
            _reloadTimer->start();
    }

    /**
     * @brief Handle the close event.
     *
     * @param event
     */
    void LogViewerDialog::closeEvent(QCloseEvent* event)
    {
        QDialog::closeEvent(event);
        _reloadTimer->stop();
    }

}   // namespace ui