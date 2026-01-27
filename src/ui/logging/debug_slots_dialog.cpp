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
#include "utils/qt_helpers.hpp"

static constexpr int COL_NAME  = 0;
static constexpr int COL_VALUE = 1;

namespace ui
{

    DebugSlotsDialog::DebugSlotsDialog(MainWindow& mainWindow)
        : QDialog(&mainWindow)
    {
        _categories        = LogManager::getInstance().getCategories();
        _currentCategories = _categories;
        _build_ui();
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

        _showOnlyModifiedCheckBox = new QCheckBox("Show only modified", this);
        _showOnlyModifiedCheckBox->setChecked(false);

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
            _defaultsButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                // set current categories to default
                _currentCategories =
                    LogManager::getInstance().getDefaultCategories();
                _populate_tree();
            }
        );

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

        connect(
            _buttonBox,
            &QDialogButtonBox::rejected,
            this,
            [this]()
            {
                // if something changed ask for confirmation
                if (_currentCategories != _categories)
                {
                    const auto res = QMessageBox::question(
                        this,
                        "Discard changes?",
                        "Are you sure you want to discard your changes?",
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::No
                    );

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
                // apply changes
                for (const auto& [category, level] : _currentCategories)
                {
                    LogManager::getInstance().setLogLevel(category, level);
                }

                accept();
            }
        );

        connect(
            _buttonBox,
            &QDialogButtonBox::clicked,
            this,
            [this](QAbstractButton* button)
            {
                if (_buttonBox->standardButton(button) ==
                    QDialogButtonBox::Apply)
                {
                    // apply changes
                    for (const auto& [category, level] : _currentCategories)
                    {
                        LogManager::getInstance().setLogLevel(category, level);
                    }

                    // update original categories
                    _categories = _currentCategories;
                }
            }
        );
    }

    void DebugSlotsDialog::_populate_tree()
    {
        _tree->blockSignals(true);
        _tree->clear();

        for (const auto& [category, level] : _currentCategories)
        {
            if (_modifiedOnly && level == _categories[category])
                continue;

            const auto categoryName =
                std::string{LogCategoryMeta::name(category)};
            const auto categoryQStr = QString::fromStdString(categoryName);

            auto* treeItem = new QTreeWidgetItem(_tree);
            treeItem->setText(COL_NAME, categoryQStr);
            treeItem->setFirstColumnSpanned(false);
            treeItem->setFlags(treeItem->flags() & ~Qt::ItemIsUserCheckable);

            auto* combo = new QComboBox(_tree);
            combo->addItems(utils::toQStringList(LogLevelMeta::names));

            const auto* it    = std::ranges::find(LogLevelMeta::values, level);
            const auto  index = std::distance(LogLevelMeta::values.begin(), it);

            combo->setCurrentIndex(static_cast<int>(index));
            _tree->setItemWidget(treeItem, COL_VALUE, combo);

            connect(
                combo,
                &QComboBox::currentIndexChanged,
                this,
                [category, this]()
                {
                    // column 2 of this row changed
                    const QString value =
                        static_cast<QComboBox*>(QObject::sender())
                            ->currentText();

                    const auto levelOpt =
                        LogLevelMeta::from_string(value.toStdString());

                    if (levelOpt.has_value())
                        _currentCategories[category] = levelOpt.value();
                }
            );
        }

        _tree->expandAll();
        _tree->blockSignals(false);
    }

}   // namespace ui