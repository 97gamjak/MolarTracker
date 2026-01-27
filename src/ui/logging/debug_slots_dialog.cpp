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
#include "ui/utils/discard_changes.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Constructs the DebugSlotsDialog
     *
     * @param mainWindow Parent main window
     */
    DebugSlotsDialog::DebugSlotsDialog(MainWindow& mainWindow)
        : QDialog(&mainWindow)
    {
        // initialize categories from global logger
        _categories        = LogManager::getInstance().getCategories();
        _currentCategories = _categories;

        _build_ui();
        _connect_buttons();
        _populate_tree();
    }

    void DebugSlotsDialog::_build_ui()
    {
        setWindowTitle("Debug / Logging Settings");
        setModal(true);
        resize(820, 560);

        _tree = new QTreeWidget(this);
        _tree->setColumnCount(2);
        _tree->setHeaderLabels({"Debug Flag", "Value"});
        _tree->setRootIsDecorated(true);
        _tree->setUniformRowHeights(true);
        auto* header = _tree->header();
        header->setSectionResizeMode(QHeaderView::Stretch);
        _tree->setAlternatingRowColors(true);

        _defaultsButton       = new QPushButton("Use default Flags", this);
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

    void DebugSlotsDialog::_connect_buttons()
    {
        connect(
            _defaultsButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                _currentCategories =
                    LogManager::getInstance().getDefaultCategories();

                _populate_tree();
            }
        );

        connect(
            _discardChangesButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                // revert current categories to original
                _currentCategories = _categories;
                _populate_tree();
            }
        );

        connect(
            _showOnlyModifiedCheckBox,
            &QCheckBox::toggled,
            this,
            [this](bool on)
            {
                _modifiedOnly = on;
                _populate_tree();
            }
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::rejected,
            this,
            [this]()
            {
                // if something changed ask for confirmation
                if (_currentCategories != _categories)
                {
                    const auto res = askDiscardChanges(this);

                    if (res == QMessageBox::No)
                        return;
                }

                reject();
            }
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::accepted,
            this,
            [this]()
            {
                for (const auto& [category, level] : _currentCategories)
                    LogManager::getInstance().setLogLevel(category, level);

                accept();
            }
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::clicked,
            this,
            [this](QAbstractButton* button)
            {
                using enum QDialogButtonBox::StandardButton;

                if (_buttonBox->standardButton(button) == Apply)
                {
                    for (const auto& [category, level] : _currentCategories)
                        LogManager::getInstance().setLogLevel(category, level);

                    _categories = _currentCategories;
                }
            }
        );
    }

    void DebugSlotsDialog::_populate_tree()
    {
        _tree->blockSignals(true);
        _tree->clear();

        static constexpr int CAT_COL   = 0;
        static constexpr int LEVEL_COL = 1;

        for (const auto& [category, level] : _currentCategories)
        {
            if (_modifiedOnly && level == _categories[category])
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

            const auto* it    = std::ranges::find(LogLevelMeta::values, level);
            const auto  index = std::distance(LogLevelMeta::values.begin(), it);

            combo->setCurrentIndex(static_cast<int>(index));
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

}   // namespace ui