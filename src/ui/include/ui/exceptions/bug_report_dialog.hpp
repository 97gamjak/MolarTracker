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
        /// The line edit for the bug report title
        QLineEdit* _titleEdit = nullptr;
        /// The plain text edit for the bug report body
        QPlainTextEdit* _bodyEdit = nullptr;

        void _buildUI(const QString& details);

       public:
        explicit BugReportDialog(
            const QString& details,
            QWidget*       parent = nullptr
        );
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__EXCEPTIONS__BUG_REPORT_DIALOG_HPP__
