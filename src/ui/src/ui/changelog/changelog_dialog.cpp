#include "ui/changelog/changelog_dialog.hpp"

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

REGISTER_LOG_CATEGORY("UI.Changelog.ChangelogDialog")

using common::makeQChild;

namespace
{
    /**
     * @brief Build the Markdown content shown in the changelog dialog's text
     * browser
     *
     * @return QString
     */
    QString buildChangelogMarkdown()
    {
        QFile file(":/changelog/CHANGELOG.md");
        if (file.open(QIODevice::ReadOnly))
            return QString::fromUtf8(file.readAll());

        LOG_ERROR("Failed to load changelog content from resource file.");
        return "# Changelog Not Found\n\nThe changelog content could not be "
               "loaded. Please ensure that the resource file is available.";
    }
}   // namespace

namespace ui
{
    /**
     * @brief Construct a new Changelog Dialog:: Changelog Dialog object
     *
     * @param parent
     */
    ChangelogDialog::ChangelogDialog(QWidget* parent) : Dialog(parent)
    {
        setWindowTitle("Changelog — MolarTracker");
        constexpr auto size = QSize(700, 500);
        resize(size);
        _buildUI();
    }

    /**
     * @brief Build the UI layout for the changelog dialog
     */
    void ChangelogDialog::_buildUI()
    {
        auto* layout = makeQChild<QVBoxLayout>(this);

        auto* titleLabel = makeQChild<QLabel>("MolarTracker Changelog", this);
        auto  font       = titleLabel->font();
        constexpr auto pointSize = 16;
        font.setPointSize(pointSize);
        font.setBold(true);
        titleLabel->setFont(font);

        _textBrowser = makeQChild<QTextBrowser>(this);
        _textBrowser->setReadOnly(true);
        _textBrowser->setOpenExternalLinks(true);
        _textBrowser->setMarkdown(buildChangelogMarkdown());

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
            &ChangelogDialog::_onExportPdf
        );
        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    }

    /**
     * @brief Export the changelog content to a PDF file
     */
    void ChangelogDialog::_onExportPdf()
    {
        const auto fileName = QFileDialog::getSaveFileName(
            this,
            "Export Changelog to PDF",
            "MolarTracker_Changelog.pdf",
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
