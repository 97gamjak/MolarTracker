#ifndef __UI__INCLUDE__UI__CHANGELOG__CHANGELOG_DIALOG_HPP__
#define __UI__INCLUDE__UI__CHANGELOG__CHANGELOG_DIALOG_HPP__

#include "ui/base/dialog.hpp"

class QTextBrowser;
class QPushButton;

namespace ui
{
    /**
     * @brief Dialog displaying the application changelog, with PDF export
     */
    class ChangelogDialog final : public Dialog
    {
        Q_OBJECT

       private:
        /// The text browser displaying the changelog
        QTextBrowser* _textBrowser = nullptr;
        /// The button to export the changelog to PDF
        QPushButton* _exportButton = nullptr;

        void _buildUI();

       private slots:
        void _onExportPdf();

       public:
        explicit ChangelogDialog(QWidget* parent);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__CHANGELOG__CHANGELOG_DIALOG_HPP__
