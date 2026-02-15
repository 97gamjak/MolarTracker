#ifndef __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__
#define __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__

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
        Settings& _settings;

        QPlainTextEdit* _textEdit;
        QPushButton*    _reloadButton;
        QCheckBox*      _autoReloadCheckBox;
        QTimer*         _reloadTimer;

       public:
        explicit LogViewerDialog(Settings& settings, QWidget* parent = nullptr);

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
        int                          _reloadIntervalMs = 1000;
        bool                         _autoReload       = false;
        QPlainTextEdit::LineWrapMode _lineWrap         = QPlainTextEdit::NoWrap;

       public:
        void              setIntervalSec(double intervalSec);
        [[nodiscard]] int getIntervalMs() const;

        void               setAutoReload(bool enabled);
        [[nodiscard]] bool isAutoReloadEnabled() const;

        void                                       setLineWrap(bool enabled);
        [[nodiscard]] QPlainTextEdit::LineWrapMode getLineWrapMode() const;
    };

}   // namespace ui

#endif   // __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__