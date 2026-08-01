#include "ui/help/help_dialog.hpp"

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPrinter>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "common/qt_helpers.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("UI.Help.HelpDialog")

using common::makeQChild;

namespace
{
    /**
     * @brief Build the HTML content shown in the help dialog's text browser
     *
     * @return QString
     */
    QString buildHelpHtml()
    {
        QFile file(":/help/settings.html");
        if (file.open(QIODevice::ReadOnly))
            return QString::fromUtf8(file.readAll());

        LOG_ERROR("Failed to load help content from resource file.");
        return "<html><body><h1>Help Content Not Found</h1><p>The help content "
               "could not be loaded. Please ensure that the resource file is "
               "available.</p></body></html>";
    }
}   // namespace

namespace ui
{
    /**
     * @brief Construct a new Help Dialog:: Help Dialog object
     *
     * @param parent
     */
    HelpDialog::HelpDialog(QWidget* parent) : Dialog(parent)
    {
        setWindowTitle("Help — MolarTracker");
        constexpr auto size = QSize(700, 500);
        resize(size);
        _buildUI();
    }

    /**
     * @brief Build the UI layout for the help dialog
     */
    void HelpDialog::_buildUI()
    {
        auto* layout = makeQChild<QVBoxLayout>(this);

        auto* titleLabel = makeQChild<QLabel>("MolarTracker Help", this);
        auto  font       = titleLabel->font();
        constexpr auto pointSize = 16;
        font.setPointSize(pointSize);
        font.setBold(true);
        titleLabel->setFont(font);

        _textBrowser = makeQChild<QTextBrowser>(this);
        _textBrowser->setReadOnly(true);
        _textBrowser->setOpenExternalLinks(true);
        _textBrowser->setHtml(buildHelpHtml());

        _exportButton     = makeQChild<QPushButton>("Export to PDF...", this);
        auto* closeButton = makeQChild<QPushButton>("Close", this);

        auto* buttonRow = makeQChild<QHBoxLayout>();
        buttonRow->addStretch();
        buttonRow->addWidget(_exportButton);
        buttonRow->addWidget(closeButton);

        layout->addWidget(titleLabel);
        layout->addWidget(_textBrowser);
        layout->addLayout(buttonRow);

        connect(
            _exportButton,
            &QPushButton::clicked,
            this,
            &HelpDialog::_onExportPdf
        );
        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    }

    /**
     * @brief Export the help content to a PDF file
     */
    void HelpDialog::_onExportPdf()
    {
        const auto fileName = QFileDialog::getSaveFileName(
            this,
            "Export Help to PDF",
            "MolarTracker_Help.pdf",
            "PDF Files (*.pdf)"
        );
        if (fileName.isEmpty())
            return;

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        _textBrowser->document()->print(&printer);
    }

}   // namespace ui
