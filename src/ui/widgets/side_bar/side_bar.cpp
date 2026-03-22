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
     * @brief Build the UI of the side bar, this will create the tree view and
     * set up the model for the side bar
     *
     */
    void SideBar::_buildUI()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _model = new QStandardItemModel(this);
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _tree = new QTreeView(this);

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

        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(index));
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

        auto* item = dynamic_cast<SideBarItem*>(_model->itemFromIndex(index));

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* menu = new QMenu(this);
        item->populateContextMenu(*menu);

        if (menu == nullptr || menu->actions().isEmpty())
            return;

        QAction* selectedAction =
            menu->exec(_tree->viewport()->mapToGlobal(pos));

        if (selectedAction != nullptr)
            emit contextMenuRequested(item, selectedAction);
    }

}   // namespace ui