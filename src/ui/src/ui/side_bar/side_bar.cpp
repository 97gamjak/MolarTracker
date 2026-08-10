#include "ui/side_bar/side_bar.hpp"

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

#include "common/qt_helpers.hpp"
#include "logging/log_macros.hpp"
#include "ui/side_bar/category.hpp"
#include "ui/side_bar/side_bar_item.hpp"

REGISTER_LOG_CATEGORY("UI.SideBar");

namespace ui
{

    /**
     * @brief Construct a new Side Bar:: Side Bar object
     *
     * @param parent The parent widget of the side bar
     *
     */
    SideBar::SideBar(QWidget* parent)
        : QWidget(parent),
          _tree(new QTreeView(this)),
          _model(new QStandardItemModel(this))
    {
        _buildUI();
    }

    /**
     * @brief Build the UI of the side bar, this will create the tree view and
     * set up the model for the side bar
     *
     */
    void SideBar::_buildUI()
    {
        auto* layout = common::makeQChild<QVBoxLayout>(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _model = common::makeQChild<QStandardItemModel>(this);
        _tree  = common::makeQChild<QTreeView>(this);

        _tree->setModel(_model);
        _tree->setHeaderHidden(true);
        _tree->setRootIsDecorated(false);
        _tree->setItemsExpandable(true);
        // _tree->setExpandsOnDoubleClick(true);
        _tree->setContextMenuPolicy(Qt::CustomContextMenu);

        layout->addWidget(_tree);

        _tree->setEditTriggers(QAbstractItemView::NoEditTriggers);

        connect(_tree, &QTreeView::clicked, this, &SideBar::_onClicked);

        connect(
            _tree,
            &QTreeView::customContextMenuRequested,
            this,
            &SideBar::_showContextMenu
        );

        connect(
            _tree,
            &QTreeView::doubleClicked,
            this,
            &SideBar::_onDoubleClicked
        );

        connect(
            _model,
            &QStandardItemModel::dataChanged,
            this,
            &SideBar::_onDataChanged
        );
    }

    /**
     * @brief Add a category to the side bar, this will create a new category
     * item and add it to the side bar
     *
     * @param category The category to add to the side bar, this should be a
     * pointer to a Category object that will be added as an item in the side
     * bar
     */
    void SideBar::addCategory(Category* category)
    {
        _model->appendRow(category);
        _tree->expand(_model->indexFromItem(category));
    }

    /**
     * @brief Remove a category from the side bar, this will remove the category
     * item from the side bar
     *
     * @param item The item to remove from the side bar, this should be a
     * pointer to a SideBarItem that is currently in the side bar, and will be
     * removed from the side bar
     */
    void SideBar::removeCategory(SideBarItem* item)
    {
        if (item == nullptr)
            return;

        if (item->parent() != nullptr)
            item->parent()->removeRow(item->row());
        else
            _model->removeRow(item->row());
    }

    /**
     * @brief Select an item in the side bar's tree view, highlighting it as
     * the currently active item without emitting the itemClicked signal.
     *
     * @param item The item to select
     */
    void SideBar::selectItem(SideBarItem* item)
    {
        if (item == nullptr)
            return;

        _tree->setCurrentIndex(_model->indexFromItem(item));
    }

    /**
     * @brief Handle the clicked signal of the tree view, this will emit the
     * itemSelected signal with the type of the item that was clicked
     *
     * @param index The index of the item that was clicked, this can be used to
     * identify which item was clicked and emit the appropriate itemSelected
     * signal
     */
    void SideBar::_onClicked(const QModelIndex& index)
    {
        if (!index.isValid())
            return;

        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(index));

        if (item == nullptr)
            return;

        LOG_DEBUG(
            std::format(
                "SideBar::_onClicked called with side bar type: {}",
                SideBarItemTypeMeta::toString(item->getType())
            )
        );

        emit itemClicked(item);
    }

    /**
     * @brief Handle the doubleClicked signal of the tree view, this will emit
     * the itemDoubleClicked signal with the type of the item that was
     * double-clicked
     *
     * @param index The index of the item that was double-clicked, this can be
     * used to identify which item was double-clicked and emit the appropriate
     * itemDoubleClicked signal
     */
    void SideBar::_onDoubleClicked(const QModelIndex& index)
    {
        if (!index.isValid())
            return;

        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(index));
        if (item == nullptr)
            return;

        if ((item->flags() & Qt::ItemIsEditable) != 0U)
        {
            _pendingRenameOldText = item->text();
            _tree->edit(index);
            return;
        }

        emit itemDoubleClicked(item);
    }
    /**
     * @brief Show the context menu of the tree view, this will emit the
     * itemContextMenuRequested signal with the type of the item that was
     * right-clicked
     *
     * @param pos The position where the context menu should be shown, this can
     * be used to identify which item was right-clicked and emit the appropriate
     * itemContextMenuRequested signal
     */
    void SideBar::_showContextMenu(const QPoint& pos)
    {
        const auto index = _tree->indexAt(pos);

        if (!index.isValid())
            return;

        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(index));

        auto* menu = common::makeQChild<QMenu>(this);
        item->populateContextMenu(*menu);

        if (menu->actions().isEmpty())
            return;

        QAction* selectedAction =
            menu->exec(_tree->viewport()->mapToGlobal(pos));

        if (selectedAction != nullptr)
            emit contextMenuRequested(item, selectedAction);
    }

    /**
     * @brief Handle the dataChanged signal of the model, this will emit the
     * itemRenameCommitted signal with the new name of the item that was
     * renamed
     *
     * @param topLeft The index of the top-left item that was changed, this can
     * be used to identify which item was renamed and emit the appropriate
     * itemRenameCommitted signal
     * @param bottomRight The index of the bottom-right item that was changed,
     * this is unused in this implementation, but is provided for completeness
     */
    void SideBar::_onDataChanged(
        const QModelIndex& topLeft,
        const QModelIndex& /*bottomRight*/
    )
    {
        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(topLeft));
        if (item == nullptr || !(item->flags() & Qt::ItemIsEditable))
            return;

        const auto newName = item->text().trimmed();
        const auto oldName = _pendingRenameOldText;

        {
            QSignalBlocker blocker(_model);
            item->setText(oldName);   // revert; controller decides if it sticks
        }

        if (newName.isEmpty() || newName == oldName)
            return;

        emit itemRenameCommitted(item, newName);
    }

}   // namespace ui
