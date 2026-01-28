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
#include "ui/main_window.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_logging
#include "logging/log_macros.hpp"

namespace ui
{

    LogViewerDialog::LogViewerDialog(MainWindow& mainWindow)
        : QDialog(&mainWindow),
          _textEdit(new QPlainTextEdit(this)),
          _reloadButton(new QPushButton(tr("Reload"), this)),
          _autoReloadCheckBox(new QCheckBox(tr("Auto Reload"), this)),
          _reloadTimer(new QTimer(this))
    {
        setWindowTitle(tr("Application Log"));
        resize(900, 600);

        _textEdit->setReadOnly(true);
        // TODO: maybe this should be configurable
        _textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        _textEdit->setMaximumBlockCount(50000);

        _autoReloadCheckBox->setChecked(false);
        // TODO: make this changeable via config and dialog
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

    void LogViewerDialog::_loadLogFile()
    {
        const auto& logFilePath =
            LogManager::getInstance().getCurrentLogFilePath();
        const auto absPath  = std::filesystem::absolute(logFilePath);
        const auto qStrPath = QString::fromStdString(absPath.string());

        // we need to flush here to ensure all logs are written before reading
        // TODO: think of a clever flushing strategy to avoid performance issues
        // especially when auto-reload is enabled
        LogManager::getInstance().flush();

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
        // TODO: think of a better way to handle this
        // maybe only scroll to bottom if user did not scroll up manually
        auto cursor = _textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        _textEdit->setTextCursor(cursor);
    }

    void LogViewerDialog::reloadLog() { _loadLogFile(); }

    void LogViewerDialog::hideEvent(QHideEvent* event)
    {
        QDialog::hideEvent(event);
        if (_autoReloadCheckBox->isChecked())
            _reloadTimer->stop();
    }

    void LogViewerDialog::showEvent(QShowEvent* event)
    {
        QDialog::showEvent(event);
        if (_autoReloadCheckBox->isChecked())
            _reloadTimer->start();
    }

    void LogViewerDialog::closeEvent(QCloseEvent* event)
    {
        QDialog::closeEvent(event);
        if (_autoReloadCheckBox->isChecked())
            _reloadTimer->stop();
    }

}   // namespace ui