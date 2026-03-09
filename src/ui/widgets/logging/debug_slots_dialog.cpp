#include "debug_slots_dialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

#include "debug_slots_log_level_delegate.hpp"
#include "debug_slots_model.hpp"
#include "logging/log_macros.hpp"
#include "ui/widgets/utils/discard_changes.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("UI.Widgets.Logging.DebugSlotsDialog");

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
    void DebugSlotsDialog::setCategories(
        const logging::LogCategories& categories
    )
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
        const logging::LogCategories& categories,
        bool                          overrideReference
    )
    {
        if (overrideReference)
        {
            _categories = categories;
            _model->setReferenceCategories(categories);
        }

        _currentCategories = categories;
        _model->setCategories(categories);
    }

    /**
     * @brief Build the user interface for the dialog
     *
     */
    void DebugSlotsDialog::_buildUi()
    {
        setWindowTitle("Logging Debug Flags");
        resize(820, 560);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _tree = new QTreeView(this);
        _tree->setRootIsDecorated(true);
        _tree->setUniformRowHeights(true);
        _tree->setEditTriggers(
            QAbstractItemView::CurrentChanged |
            QAbstractItemView::SelectedClicked
        );
        auto* header = _tree->header();
        header->setSectionResizeMode(QHeaderView::Stretch);
        _tree->setAlternatingRowColors(true);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _model = new LogCategoryModel(_currentCategories, _tree);
        _tree->setModel(_model);

        _tree->setItemDelegateForColumn(
            LogCategoryModel::getLogLevelColumn(),
            new DebugSlotsLogLevelDelegate(this)
        );

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* applyToChildrenDelegate =
            new DebugSlotsApplyToChildrenDelegate(this);
        _tree->setItemDelegateForColumn(
            LogCategoryModel::getApplyToChildrenColumn(),
            applyToChildrenDelegate
        );

        connect(
            applyToChildrenDelegate,
            &DebugSlotsApplyToChildrenDelegate::applyToChildrenRequested,
            this,
            &DebugSlotsDialog::_applyToChildren
        );

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _defaultsButton = new QPushButton("Use default flags", this);
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _discardChangesButton = new QPushButton("Discard Changes", this);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _showOnlyModifiedCheckBox = new QCheckBox("Show only modified", this);
        _showOnlyModifiedCheckBox->setChecked(false);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel |
                QDialogButtonBox::Apply,
            this
        );

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* upperBottomRow = new QHBoxLayout();
        upperBottomRow->addWidget(_showOnlyModifiedCheckBox);
        upperBottomRow->addStretch(1);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* bottomRow = new QHBoxLayout();
        bottomRow->addWidget(_defaultsButton);
        bottomRow->addWidget(_discardChangesButton);
        bottomRow->addStretch(1);
        bottomRow->addWidget(_buttonBox);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
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
            [this](bool checked)
            {
                _modifiedOnly = checked;
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

        _model->setShowModifiedOnly(_modifiedOnly);

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
        setCategories(_categories);
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
        auto isModified = false;
        for (const auto& category : _currentCategories.getCategories())
        {
            const auto categoryOpt =
                _categories.getCategory(category.getName());

            if (!categoryOpt.has_value())
                continue;

            if (category.getLogLevel() != categoryOpt->getLogLevel())
            {
                isModified = true;
                break;
            }
        }

        if (isModified)
        {
            const auto res = askDiscardChanges(this);

            if (res == QMessageBox::No)
                return;

            // revert changes
            setCategories(_categories);
        }

        reject();
    }

    /**
     * @brief Apply the log level change to all child categories of the given
     * index in the tree view
     *
     * @param idx The model index of the category for which to apply the log
     * level change to its children
     */
    void DebugSlotsDialog::_applyToChildren(const QModelIndex& idx)
    {
        _model->applyToChildren(idx);
    }

}   // namespace ui