#ifndef __UI__INCLUDE__UI__HELP__HELP_DIALOG_HPP__
#define __UI__INCLUDE__UI__HELP__HELP_DIALOG_HPP__

#include "ui/base/dialog.hpp"

class QTextBrowser;
class QPushButton;

namespace ui
{
    /**
     * @brief Dialog displaying the application help page, with PDF export
     */
    class HelpDialog final : public Dialog
    {
        Q_OBJECT

       private:
        QTextBrowser* _textBrowser  = nullptr;
        QPushButton*  _exportButton = nullptr;

        void _buildUI();

       private slots:
        void _onExportPdf();

       public:
        explicit HelpDialog(QWidget* parent);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__HELP__HELP_DIALOG_HPP__
