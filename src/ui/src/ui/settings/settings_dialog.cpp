#include "ui/settings/settings_dialog.hpp"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qstackedwidget.h>

#include <QPushButton>

#include "settings/params/param_utils.hpp"
#include "settings/settings.hpp"
#include "ui/settings/settings_section.hpp"
#include "ui/settings/settings_sidebar.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    SettingsDialog::SettingsDialog(
        settings::Settings& settings,
        QWidget*            parent
    )
        : QDialog(parent, Qt::Dialog), _settings(settings)
    {
        setWindowTitle("Settings");
        setSizeGripEnabled(false);

        _build();
        _applyStyleSheet();
    }

    void SettingsDialog::_build()
    {
        // ── Root layout
        // ───────────────────────────────────────────────────────────
        auto* root = utils::makeQChild<QVBoxLayout>(this);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        // ── Body (sidebar + stack)
        // ────────────────────────────────────────────────
        auto* body       = utils::makeQChild<QWidget>(this);
        auto* bodyLayout = utils::makeQChild<QHBoxLayout>(body);
        bodyLayout->setContentsMargins(0, 0, 0, 0);
        bodyLayout->setSpacing(0);

        _sidebar = utils::makeQChild<SettingsSidebar>(body);
        _sidebar->setFixedWidth(160);

        _stack = utils::makeQChild<QStackedWidget>(body);

        bodyLayout->addWidget(_sidebar);

        // Vertical separator
        auto* sep = utils::makeQChild<QFrame>(body);
        sep->setFrameShape(QFrame::VLine);
        sep->setObjectName("bodySeparator");
        bodyLayout->addWidget(sep);

        bodyLayout->addWidget(_stack, 1);

        root->addWidget(body, 1);

        // ── Bottom bar
        // ────────────────────────────────────────────────────────────
        auto* bottomBar = utils::makeQChild<QWidget>(this);
        bottomBar->setObjectName("bottomBar");
        auto* bottomLayout = utils::makeQChild<QHBoxLayout>(bottomBar);
        bottomLayout->setContentsMargins(16, 8, 16, 8);
        bottomLayout->setSpacing(8);

        _unsavedLabel = utils::makeQChild<QLabel>(bottomBar);
        _unsavedLabel->setObjectName("unsavedLabel");
        _unsavedLabel->setText("● unsaved changes");
        _unsavedLabel->setVisible(false);
        bottomLayout->addWidget(_unsavedLabel);
        bottomLayout->addStretch();

        auto* saveBtn = utils::makeQChild<QPushButton>(bottomBar);
        saveBtn->setText("Save");
        saveBtn->setObjectName("saveButton");
        saveBtn->setDefault(true);

        auto* closeBtn = utils::makeQChild<QPushButton>(bottomBar);
        closeBtn->setText("Close");
        closeBtn->setObjectName("closeButton");

        bottomLayout->addWidget(saveBtn);
        bottomLayout->addWidget(closeBtn);

        root->addWidget(bottomBar);

        // ── Wire buttons
        // ──────────────────────────────────────────────────────────
        connect(
            saveBtn,
            &QPushButton::clicked,
            this,
            [this]()
            {
                _settings.save();
                _updateUnsavedLabel();
                emit saveRequested();
            }
        );

        connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

        // ── Build sections
        // ──────────────────────────────────────────────────────── Each section
        // widget is templated on its concrete container type. Order here must
        // match kGeneralIndex / kUiIndex / kLoggingIndex constants.

        // Static trampoline — no capture needed
        static constexpr auto onSectionDirty =
            [](void* userData, const bool& isDirty)
        {
            auto* ctx = static_cast<DirtyContext*>(userData);
            ctx->dialog->_sidebar->setSectionDirty(ctx->index, isDirty);
            ctx->dialog->_updateUnsavedLabel();
        };

        auto addSection = [&]<settings::IsParamContainer TSection>(
                              const QString& title,
                              TSection&      section,
                              int            index
                          )
        {
            _sidebar->addSection(title);

            auto* sectionWidget =
                utils::makeQChild<SettingsSection<TSection>>(section);
            _stack->addWidget(sectionWidget);

            // Aggregate dirty connections from the section widget
            for (auto& conn : sectionWidget->getConnections())
                _connections.push_back(std::move(conn));

            // Heap-allocate context owned by _dirtyContexts
            auto  ctx    = std::make_unique<DirtyContext>(this, index);
            auto* ctxPtr = ctx.get();
            _dirtyContexts.push_back(std::move(ctx));

            auto containerConnections = section.subscribeToDirty(
                static_cast<OnDirtyChanged::func>(onSectionDirty),
                static_cast<void*>(ctxPtr)
            );

            for (auto& conn : containerConnections)
                _connections.push_back(std::move(conn));
        };

        addSection("General", _settings.getGeneralSettings(), kGeneralIndex);
        addSection("Interface", _settings.getUISettings(), kUiIndex);
        addSection("Logging", _settings.getLoggingSettings(), kLoggingIndex);

        // ── Sidebar navigation
        // ────────────────────────────────────────────────────
        _sidebar->setOnSectionSelected([this](int index)
                                       { _stack->setCurrentIndex(index); });

        _sidebar->selectSection(kGeneralIndex);
    }

    void SettingsDialog::_updateUnsavedLabel()
    {
        // Any section dirty → show the label
        const bool anyDirty = _settings.getGeneralSettings().isDirty() ||
                              _settings.getUISettings().isDirty() ||
                              _settings.getLoggingSettings().isDirty();

        _unsavedLabel->setVisible(anyDirty);
    }

    void SettingsDialog::_applyStyleSheet()
    {
        setStyleSheet(R"(
        /* ── Dialog ─────────────────────────────────── */
        SettingsDialog {
            background: #0e0e0e;
        }

        /* ── Sidebar ─────────────────────────────────── */
        QListWidget#settingsSidebar {
            background: #111111;
            border: none;
            padding: 8px 0;
            font-family: "Courier New", monospace;
            font-size: 12px;
            color: #6a7a8a;
            outline: none;
        }
        QListWidget#settingsSidebar::item {
            padding: 8px 16px;
            border-radius: 0;
        }
        QListWidget#settingsSidebar::item:selected {
            background: #1a1a1a;
            color: #c8d8ec;
            border-left: 2px solid #4a9eff;
        }
        QListWidget#settingsSidebar::item:hover:!selected {
            background: #161616;
            color: #8a9ab0;
        }

        /* ── Body separator ──────────────────────────── */
        QFrame#bodySeparator {
            color: #1e1e1e;
        }

        /* ── Section header ──────────────────────────── */
        QWidget#sectionHeader {
            background: #111111;
        }
        QLabel#sectionTitle {
            font-family: "Courier New", monospace;
            font-size: 14px;
            font-weight: bold;
            color: #c8d8ec;
        }
        QLabel#sectionDescription {
            font-family: "Courier New", monospace;
            font-size: 11px;
            color: #4a5a6a;
        }
        QFrame#sectionDivider {
            color: #1e1e1e;
        }

        /* ── Param rows ──────────────────────────────── */
        QWidget#paramRow {
            background: transparent;
        }
        QWidget#paramRow:hover {
            background: #141414;
        }
        QLabel#paramLabel {
            font-family: "Courier New", monospace;
            font-size: 12px;
            color: #8a9ab0;
            min-width: 160px;
        }

        /* Dirty stripe: blue when dirty, transparent otherwise */
        QWidget#dirtyStripe[dirty="false"] {
            background: transparent;
            border-radius: 2px;
        }
        QWidget#dirtyStripe[dirty="true"] {
            background: #4a9eff;
            border-radius: 2px;
        }

        /* ── Reboot badge ────────────────────────────── */
        QLabel#rebootBadge {
            font-family: "Courier New", monospace;
            font-size: 10px;
            color: #aa7700;
            padding: 1px 5px;
            border: 1px solid #554400;
            border-radius: 3px;
        }

        /* ── Editors ─────────────────────────────────── */
        QLineEdit, QSpinBox, QDoubleSpinBox {
            background: #1a1a1a;
            border: 1px solid #2a2a2a;
            border-radius: 3px;
            color: #c8d8ec;
            font-family: "Courier New", monospace;
            font-size: 12px;
            padding: 3px 8px;
            selection-background-color: #1a3a5a;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #4a9eff;
        }
        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: #222222;
            border: none;
            width: 16px;
        }
        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #2a6aad;
        }

        QComboBox {
            background: #1a1a1a;
            border: 1px solid #2a2a2a;
            border-radius: 3px;
            color: #c8d8ec;
            font-family: "Courier New", monospace;
            font-size: 12px;
            padding: 3px 8px;
            min-width: 110px;
        }
        QComboBox:focus {
            border-color: #4a9eff;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox QAbstractItemView {
            background: #1a1a1a;
            border: 1px solid #2a2a2a;
            color: #c8d8ec;
            selection-background-color: #1a3a5a;
            font-family: "Courier New", monospace;
            font-size: 12px;
        }

        QCheckBox {
            color: #c8d8ec;
            font-family: "Courier New", monospace;
            font-size: 12px;
            spacing: 6px;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid #3a3a3a;
            border-radius: 3px;
            background: #1a1a1a;
        }
        QCheckBox::indicator:checked {
            background: #4a9eff;
            border-color: #4a9eff;
        }
        QCheckBox::indicator:hover {
            border-color: #4a9eff;
        }

        /* ── Scroll area ─────────────────────────────── */
        QScrollArea {
            background: #0e0e0e;
            border: none;
        }
        QScrollBar:vertical {
            background: #111;
            width: 6px;
            border: none;
        }
        QScrollBar::handle:vertical {
            background: #2a2a2a;
            border-radius: 3px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #3a3a3a;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }

        /* ── Bottom bar ──────────────────────────────── */
        QWidget#bottomBar {
            background: #111111;
            border-top: 1px solid #1e1e1e;
        }
        QLabel#unsavedLabel {
            font-family: "Courier New", monospace;
            font-size: 11px;
            color: #4a9eff;
        }
        QPushButton#saveButton {
            background: #1a3a5a;
            border: 1px solid #2a6aad;
            border-radius: 3px;
            color: #9acaff;
            font-family: "Courier New", monospace;
            font-size: 12px;
            padding: 5px 20px;
            min-width: 70px;
        }
        QPushButton#saveButton:hover {
            background: #2a6aad;
        }
        QPushButton#saveButton:pressed {
            background: #1a4a8a;
        }
        QPushButton#closeButton {
            background: transparent;
            border: 1px solid #2a2a2a;
            border-radius: 3px;
            color: #6a7a8a;
            font-family: "Courier New", monospace;
            font-size: 12px;
            padding: 5px 20px;
            min-width: 70px;
        }
        QPushButton#closeButton:hover {
            background: #1a1a1a;
            color: #8a9ab0;
        }
    )");
    }

}   // namespace ui