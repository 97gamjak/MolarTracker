#ifndef __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__
#define __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__

#include <QDialog>
#include <QPlainTextEdit>
#include <memory>

class QPlainTextEdit;   // Forward declaration
class QPushButton;      // Forward declaration
class QCheckBox;        // Forward declaration
class QTimer;           // Forward declaration
class QWidget;          // Forward declaration

namespace ui
{

    /**
     * @brief Dialog for viewing application log files.
     *
     */
    class LogViewerDialog final : public QDialog
    {
        Q_OBJECT

       public:
        class Settings;

       private:
        /// Settings for the log viewer dialog
        std::shared_ptr<Settings> _settings;

        /// The text edit widget for displaying log contents
        QPlainTextEdit* _textEdit;
        /// Button for manually reloading the log file
        QPushButton* _reloadButton;
        /// Checkbox for enabling/disabling auto-reload
        QCheckBox* _autoReloadCheckBox;
        /// Timer for auto-reloading the log file
        QTimer* _reloadTimer;

       public:
        explicit LogViewerDialog(
            std::shared_ptr<Settings> settings,
            QWidget*                  parent
        );

       protected:
        void hideEvent(QHideEvent* event) override;
        void showEvent(QShowEvent* event) override;
        void closeEvent(QCloseEvent* event) override;

       private slots:
        void reloadLog();

       private:
        void _loadLogFile();
    };

    /**
     * @brief Settings for LogViewerDialog
     *
     */
    struct LogViewerDialog::Settings
    {
        /// The interval for auto-reloading the log file, in milliseconds
        int reloadIntervalMs;

        /// Whether auto-reload is enabled
        bool autoReload;

        /// The line wrap mode for the text edit widget
        QPlainTextEdit::LineWrapMode lineWrap;

        /// The size of the log viewer dialog in pixels, represented as a tuple
        /// of (width, height)
        std::pair<int, int> dialogSize;

       public:
        Settings() = delete;
        explicit Settings(
            int                 reloadIntervalMs,
            bool                autoReload,
            bool                lineWrap,
            std::pair<int, int> dialogSize
        );
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__