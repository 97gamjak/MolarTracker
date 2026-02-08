#include "debug_slots_dialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "logging/log_manager.hpp"
#include "logging/logging_base.hpp"
#include "ui/main_window.hpp"
#include "ui/widgets/utils/discard_changes.hpp"
#include "utils/qt_helpers.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_logging
#include "logging/log_macros.hpp"

namespace ui
{

    /**
     * @brief Constructs the DebugSlotsDialog
     *
     * @param parent widget
     */
    DebugSlotsDialog::DebugSlotsDialog(QWidget* parent) : QDialog(parent)
    {
        _buildUi();
        _connectButtons();
    }

    /**
     * @brief Set the debug flag categories to display and edit in the dialog
     *
     * @param categories The debug flag categories to set
     */
    void DebugSlotsDialog::setCategories(const LogCategoryMap& categories)
    {
        setCategories(categories, true);
    }

    /**
     * @brief Set the debug flag categories to display and edit in the dialog
     *
     * @param categories The debug flag categories to set
     * @param overrideReference Whether to override the reference categories
     * with the new ones
     */
    void DebugSlotsDialog::setCategories(
        const LogCategoryMap& categories,
        const bool            overrideReference
    )
    {
        if (overrideReference)
            _categories = categories;

        _currentCategories = categories;
    }

    /**
     * @brief Build the user interface for the dialog
     *
     */
    void DebugSlotsDialog::_buildUi()
    {
        setWindowTitle("Logging Debug Flags");
        resize(820, 560);

        _tree = new QTreeWidget(this);
        _tree->setColumnCount(2);
        _tree->setHeaderLabels({"Debug Flag", "Value"});
        _tree->setRootIsDecorated(true);
        _tree->setUniformRowHeights(true);
        auto* header = _tree->header();
        header->setSectionResizeMode(QHeaderView::Stretch);
        _tree->setAlternatingRowColors(true);

        _defaultsButton       = new QPushButton("Use default flags", this);
        _discardChangesButton = new QPushButton("Discard Changes", this);

        _showOnlyModifiedCheckBox = new QCheckBox("Show only modified", this);
        _showOnlyModifiedCheckBox->setChecked(false);

        _buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel |
                QDialogButtonBox::Apply,
            this
        );

        // bottom row
        auto* upperBottomRow = new QHBoxLayout();
        upperBottomRow->addWidget(_showOnlyModifiedCheckBox);
        upperBottomRow->addStretch(1);

        auto* bottomRow = new QHBoxLayout();
        bottomRow->addWidget(_defaultsButton);
        bottomRow->addWidget(_discardChangesButton);
        bottomRow->addStretch(1);
        bottomRow->addWidget(_buttonBox);

        auto* root = new QVBoxLayout();
        root->addWidget(_tree, 1);
        root->addLayout(upperBottomRow);
        root->addLayout(bottomRow);
        setLayout(root);
    }

    /**
     * @brief Connect the buttons to their respective slots
     *
     */
    void DebugSlotsDialog::_connectButtons()
    {
        connect(
            _defaultsButton,
            &QPushButton::clicked,
            this,
            &DebugSlotsDialog::_emitDefaults
        );

        connect(
            _discardChangesButton,
            &QPushButton::clicked,
            this,
            &DebugSlotsDialog::_discardChanges
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &DebugSlotsDialog::_rejectChanges
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &DebugSlotsDialog::_emitApplyAndClose
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::clicked,
            this,
            [this](QAbstractButton* button)
            {
                using enum QDialogButtonBox::StandardButton;

                if (_buttonBox->standardButton(button) == Apply)
                    _emitApply();
            }
        );

        connect(
            _showOnlyModifiedCheckBox,
            &QCheckBox::toggled,
            this,
            [this](bool on)
            {
                _modifiedOnly = on;
                populateTree();
            }
        );
    }

    /**
     * @brief Populate the tree widget with the current debug flag categories
     * and their values
     *
     */
    void DebugSlotsDialog::populateTree()
    {
        _tree->blockSignals(true);
        _tree->clear();

        static constexpr int CAT_COL   = 0;
        static constexpr int LEVEL_COL = 1;

        for (const auto& [category, level] : _currentCategories)
        {
            if (_modifiedOnly && level == _categories.at(category))
                continue;

            const auto categoryName = LogCategoryMeta::name(category);
            const auto categoryStr  = std::string(categoryName);
            const auto categoryQStr = QString::fromStdString(categoryStr);

            auto* treeItem = new QTreeWidgetItem(_tree);
            treeItem->setText(CAT_COL, categoryQStr);
            treeItem->setFirstColumnSpanned(false);
            treeItem->setFlags(treeItem->flags() & ~Qt::ItemIsUserCheckable);

            auto* combo = new QComboBox(_tree);
            combo->addItems(utils::toQStringList(LogLevelMeta::names));

            const auto indexOpt = LogLevelMeta::index(level);

            if (!indexOpt.has_value())
                LOG_ERROR(
                    "LogLevel for category " + categoryStr +
                    " is invalid, defaulting to Off"
                );

            combo->setCurrentIndex(static_cast<int>(indexOpt.value_or(0)));
            _tree->setItemWidget(treeItem, LEVEL_COL, combo);

            connect(
                combo,
                &QComboBox::currentIndexChanged,
                this,
                [category, this]()
                {
                    // column 2 of this row changed
                    const auto levelText =
                        static_cast<QComboBox*>(QObject::sender())
                            ->currentText()
                            .toStdString();

                    const auto levelOpt = LogLevelMeta::from_string(levelText);

                    if (levelOpt.has_value())
                        _currentCategories[category] = levelOpt.value();
                }
            );
        }

        _tree->expandAll();
        _tree->blockSignals(false);
    }

    /**
     * @brief Emit the requested action with the current debug flag categories
     *
     * @param action The action that was requested (apply changes, reset to
     * default, etc.)
     */
    void DebugSlotsDialog::_emit(const Action& action)
    {
        emit requested(action, _currentCategories);
    }

    /**
     * @brief Emit the apply changes action with the current debug flag
     * categories
     *
     * @note This will apply the changes without closing the dialog
     */
    void DebugSlotsDialog::_emitApply() { _emit(Action::Apply); }

    /**
     * @brief Emit the apply changes and close action with the current debug
     * flag categories
     *
     * @note This will apply the changes and close the dialog
     */
    void DebugSlotsDialog::_emitApplyAndClose()
    {
        _emit(Action::ApplyAndClose);
    }

    /**
     * @brief Emit the reset to default action
     *
     * @note This will reset the debug flag categories to their default values
     * without closing the dialog
     */
    void DebugSlotsDialog::_emitDefaults() { _emit(Action::ResetDefault); }

    /**
     * @brief Discard changes and reset the current categories to the reference
     * categories, then repopulate the tree
     *
     * @note This will discard any unsaved changes and reset the current
     * categories to the reference categories, which are usually the values
     * that were loaded when the dialog was opened or when the last apply action
     * was performed. After resetting, it will repopulate the tree to reflect
     * the discarded changes.
     */
    void DebugSlotsDialog::_discardChanges()
    {
        _currentCategories = _categories;
        populateTree();
    }

    /**
     * @brief Reject changes and close the dialog
     *
     * @note If there are unsaved changes, it will ask for confirmation before
     * closing. If the user confirms, it will discard the changes and close the
     * dialog. If the user cancels, it will keep the dialog open.
     */
    void DebugSlotsDialog::_rejectChanges()
    {
        // if something changed ask for confirmation
        if (_currentCategories != _categories)
        {
            const auto res = askDiscardChanges(this);

            if (res == QMessageBox::No)
                return;

            // revert changes
            _currentCategories = _categories;
        }

        reject();
    }

}   // namespace ui