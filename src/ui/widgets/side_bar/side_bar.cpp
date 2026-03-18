#include "side_bar.hpp"

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

#include "category.hpp"
#include "side_bar_item.hpp"

namespace ui
{
    /**
     * @brief Construct a new Side Bar:: Side Bar object
     *
     */
    SideBar::SideBar(QWidget* parent)
        : QWidget(parent),
          _tree(new QTreeView(this)),
          _model(new QStandardItemModel(this))
    {
        _buildUI();
    }

    void SideBar::addCategory(Category* category)
    {
        _model->appendRow(category);
        _tree->expand(_model->indexFromItem(category));
    }

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
     * @brief Build the UI of the side bar, this will create the tree view and
     * set up the model for the side bar
     *
     */
    void SideBar::_buildUI()
    {
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _model = new QStandardItemModel(this);
        _tree  = new QTreeView(this);

        _tree->setModel(_model);
        _tree->setHeaderHidden(true);
        _tree->setRootIsDecorated(false);
        _tree->setItemsExpandable(true);
        // _tree->setExpandsOnDoubleClick(true);
        _tree->setContextMenuPolicy(Qt::CustomContextMenu);

        layout->addWidget(_tree);

        connect(_tree, &QTreeView::clicked, this, &SideBar::_onClicked);

        connect(
            _tree,
            &QTreeView::customContextMenuRequested,
            this,
            &SideBar::_showContextMenu
        );
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

        auto* item = static_cast<SideBarItem*>(_model->itemFromIndex(index));
        emit  itemClicked(item);
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

        auto*  item = static_cast<SideBarItem*>(_model->itemFromIndex(index));
        QMenu* menu = nullptr;
        item->populateContextMenu(*menu);

        QAction* selectedAction =
            menu->exec(_tree->viewport()->mapToGlobal(pos));

        if (selectedAction != nullptr)
            emit contextMenuRequested(item, selectedAction);
    }

}   // namespace ui