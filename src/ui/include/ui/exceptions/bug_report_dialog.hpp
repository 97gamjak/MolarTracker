#ifndef __UI__INCLUDE__UI__EXCEPTIONS__BUG_REPORT_DIALOG_HPP__
#define __UI__INCLUDE__UI__EXCEPTIONS__BUG_REPORT_DIALOG_HPP__

#include "ui/base/dialog.hpp"

class QLineEdit;
class QPlainTextEdit;

namespace ui
{
    /**
     * @brief Dialog that lets the user review and edit a pre-filled bug
     * report before opening GitHub's "New issue" page in the browser
     */
    class BugReportDialog final : public Dialog
    {
        Q_OBJECT

       private:
        QLineEdit*      _titleEdit = nullptr;
        QPlainTextEdit* _bodyEdit  = nullptr;

        void _buildUI(const QString& details);

       public:
        explicit BugReportDialog(
            const QString& details,
            QWidget*       parent = nullptr
        );
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__EXCEPTIONS__BUG_REPORT_DIALOG_HPP__
