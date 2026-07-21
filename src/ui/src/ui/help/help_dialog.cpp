#include "ui/help/help_dialog.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPrinter>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace
{
    /**
     * @brief Build the HTML content shown in the help dialog's text browser
     *
     * @return QString
     */
    QString buildHelpHtml()
    {
        return QStringLiteral(R"html(
<p>This page collects usage documentation for MolarTracker's dialogs and
features.</p>

<h3>Settings</h3>
<p>Open the settings dialog via <b>Settings &rarr; Preferences</b> in the menu
bar. It lets you configure general, UI, and logging preferences.</p>

<p align="center"><img src=":/help/settings_menu_location.png" width="500"></p>

<p>The dialog opens on the <b>General Settings</b> section by default:</p>

<p align="center"><img src=":/help/settings_dialog_general.png" width="600"></p>

<p>The sidebar on the left lists the top-level sections:</p>
<ul>
  <li><b>General Settings</b> &mdash; app version, default profile, dismissed
      update notices</li>
  <li><b>UI Settings</b> &mdash; grouped into <i>Profile UI Settings</i> and
      <i>Log Viewer Settings</i>, shown as an overview page with cards linking
      to each sub-section</li>
  <li><b>Logging Settings</b> &mdash; log file rotation, retention, and
      default log level</li>
</ul>

<p align="center"><img src=":/help/settings_dialog_ui_overview.png" width="600"></p>

<p>Editing a value updates it immediately in memory &mdash; a dot appears next
to the affected section in the sidebar, and an "unsaved changes" label
appears at the bottom. Click <b>Save</b> to persist your changes to disk, or
<b>Close</b> to discard them and revert to how they were before you opened
the dialog.</p>
)html");
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
