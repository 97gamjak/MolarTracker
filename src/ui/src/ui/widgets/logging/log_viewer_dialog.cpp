#include "ui/logging/log_viewer_dialog.hpp"

#include <QCheckBox>
#include <QFile>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <filesystem>

#include "logging/log_macros.hpp"
#include "logging/log_manager.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("UI.Logging.LogViewerDialog");

namespace ui
{

    /**
     * @brief Construct a new Log Viewer Dialog:: Log Viewer Dialog object
     *
     * @param settings Settings for the log viewer dialog
     * @param parent Parent widget
     */
    LogViewerDialog::LogViewerDialog(
        std::shared_ptr<Settings> settings,
        QWidget*                  parent
    )
        : QDialog(parent),
          _settings(std::move(settings)),
          _textEdit(new QPlainTextEdit(this)),
          _reloadButton(new QPushButton(tr("Reload"), this)),
          _autoReloadCheckBox(new QCheckBox(tr("Auto Reload"), this)),
          _reloadTimer(new QTimer(this))
    {
        setWindowTitle(tr("Application Log"));
        resize(_settings->dialogSize.first, _settings->dialogSize.second);

        _textEdit->setReadOnly(true);

        _textEdit->setLineWrapMode(_settings->lineWrap);
        _textEdit->setMaximumBlockCount(_settings->maxBlockCount);

        _autoReloadCheckBox->setChecked(_settings->autoReload);
        _reloadTimer->setInterval(_settings->reloadIntervalMs);

        auto* buttonLayout = utils::makeQChild<QHBoxLayout>(this);
        buttonLayout->addWidget(_reloadButton);
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(_autoReloadCheckBox);

        auto* layout = utils::makeQChild<QVBoxLayout>(this);
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
        auto&       logManager  = logging::LogManager::getInstance();
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

    /**
     * @brief Construct a new Log Viewer Dialog:: Settings:: Settings object
     *
     * @param _reloadIntervalMs
     * @param _autoReload
     * @param _lineWrap
     * @param _dialogSize
     * @param _maxBlockCount
     */
    LogViewerDialog::Settings::Settings(
        int                 _reloadIntervalMs,
        bool                _autoReload,
        bool                _lineWrap,
        std::pair<int, int> _dialogSize,
        int                 _maxBlockCount
    )
        : reloadIntervalMs(_reloadIntervalMs),
          autoReload(_autoReload),
          lineWrap(
              _lineWrap ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap
          ),
          dialogSize(_dialogSize),
          maxBlockCount(_maxBlockCount)
    {
    }

}   // namespace ui