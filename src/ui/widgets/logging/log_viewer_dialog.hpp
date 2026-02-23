#ifndef __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__
#define __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__

#include <QDialog>
#include <QPlainTextEdit>

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
        /// TODO: we might want to use here only some sub-settings
        /// Reference to settings
        Settings& _settings;

        /// The text edit widget for displaying log contents
        QPlainTextEdit* _textEdit;
        /// Button for manually reloading the log file
        QPushButton* _reloadButton;
        /// Checkbox for enabling/disabling auto-reload
        QCheckBox* _autoReloadCheckBox;
        /// Timer for auto-reloading the log file
        QTimer* _reloadTimer;

       public:
        explicit LogViewerDialog(Settings& settings, QWidget* parent);

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
    class LogViewerDialog::Settings
    {
       private:
        /// The interval for auto-reloading the log file, in milliseconds
        int _reloadIntervalMs = 1000;

        /// Whether auto-reload is enabled
        bool _autoReload = false;

        /// The line wrap mode for the text edit widget
        QPlainTextEdit::LineWrapMode _lineWrap = QPlainTextEdit::NoWrap;

       public:
        void              setIntervalSec(double intervalSec);
        [[nodiscard]] int getIntervalMs() const;

        void               setAutoReload(bool autoReload);
        [[nodiscard]] bool isAutoReloadEnabled() const;

        void                                       setLineWrap(bool enabled);
        [[nodiscard]] QPlainTextEdit::LineWrapMode getLineWrapMode() const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__LOG_VIEWER_DIALOG_HPP__