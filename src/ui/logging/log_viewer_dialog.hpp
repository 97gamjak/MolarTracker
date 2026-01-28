#ifndef __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__
#define __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__

#include <QDialog>

class QPlainTextEdit;   // Forward declaration
class QPushButton;      // Forward declaration
class QCheckBox;        // Forward declaration
class QTimer;           // Forward declaration

namespace ui
{
    class MainWindow;   // Forward declaration

    class LogViewerDialog final : public QDialog
    {
        Q_OBJECT

       private:
        QPlainTextEdit* _textEdit;
        QPushButton*    _reloadButton;
        QCheckBox*      _autoReloadCheckBox;
        QTimer*         _reloadTimer;

       public:
        explicit LogViewerDialog(MainWindow& parent);

       protected:
        void hideEvent(QHideEvent* event) override;
        void showEvent(QShowEvent* event) override;
        void closeEvent(QCloseEvent* event) override;

       private slots:
        void reloadLog();

       private:
        void _loadLogFile();
    };

}   // namespace ui

#endif   // __UI__LOGGING__LOG_VIEWER_DIALOG_HPP__