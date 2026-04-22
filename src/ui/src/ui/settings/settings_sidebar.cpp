#include "ui/settings/settings_sidebar.hpp"

#include <QTreeWidget>

#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Settings Sidebar:: Settings Sidebar object
     *
     * @param parent
     */
    SettingsSidebar::SettingsSidebar(QWidget* parent) : QWidget(parent)
    {
        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _tree = utils::makeQChild<QTreeWidget>(this);
        _tree->setObjectName("settingsSidebar");
        _tree->setHeaderHidden(true);
        _tree->setFrameShape(QFrame::NoFrame);
        _tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        constexpr std::size_t indentation = 12;
        _tree->setIndentation(indentation);
        _tree->setAnimated(true);

        connect(
            _tree,
            &QTreeWidget::itemClicked,
            [this](QTreeWidgetItem* item, int column)
            { _onItemClicked(item, column); }
        );

        layout->addWidget(_tree);
    }

    /**
     * @brief Add a top-level section entry to the sidebar, this is used for
     * sections that have no subsections (leaf sections), the stackIndex is
     * stored in the item's data for later retrieval when the item is clicked
     *
     * @param title The title of the section to add to the sidebar
     * @param stackIndex The index of the corresponding page in the
     * QStackedWidget
     */
    void SettingsSidebar::addTopLevel(const QString& title, int stackIndex)
    {
        auto* item = utils::makeQChild<QTreeWidgetItem>(_tree);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
    }

    /**
     * @brief Add a parent section entry to the sidebar, this is used for
     * sections that have subsections (non-leaf sections), the stackIndex is
     * stored in the item's data for later retrieval when the item is clicked,
     * the returned QTreeWidgetItem pointer can be used to add child entries for
     * subsections using addChild
     *
     * @param title The title of the section to add to the sidebar
     * @param stackIndex The index of the corresponding page in the
     * QStackedWidget for the overview page of this section
     * @return QTreeWidgetItem* A pointer to the created tree widget item, used
     * for adding child items for subsections
     */
    QTreeWidgetItem* SettingsSidebar::addParent(
        const QString& title,
        int            stackIndex
    )
    {
        auto* item = utils::makeQChild<QTreeWidgetItem>(_tree);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
        item->setExpanded(true);
        return item;
    }

    /**
     * @brief Add a child section entry to the sidebar under the given parent
     * item, this is used for subsections, the stackIndex is stored in the
     * item's data for later retrieval when the item is clicked
     *
     * @param parent A pointer to the parent QTreeWidgetItem to add this child
     * under
     * @param title The title of the subsection to add to the sidebar
     * @param stackIndex The index of the corresponding page in the
     * QStackedWidget for this subsection
     */
    void SettingsSidebar::addChild(
        QTreeWidgetItem* parent,
        const QString&   title,
        int              stackIndex
    )
    {
        auto* item = utils::makeQChild<QTreeWidgetItem>(parent);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
    }

    /**
     * @brief Set the dirty state of a section in the sidebar, this updates the
     * section title to include a red dot if dirty is true, or to remove the
     * dot if dirty is false, it uses the base title stored in the item's data
     * to ensure the dot is added/removed correctly without affecting the rest
     * of the title text
     *
     * @param index The stack index of the section to update, used to find the
     * corresponding item in the tree
     * @param dirty The new dirty state of the section, true if there are
     * uncommitted changes in that section, false otherwise
     */
    void SettingsSidebar::setSectionDirty(int index, bool dirty)
    {
        auto* item = _findByStackIndex(index);
        if (item == nullptr)
            return;

        const QString base = item->data(0, kBaseTitleRole).toString();
        item->setText(0, dirty ? base + "  ●" : base);
    }

    /**
     * @brief Select a section in the sidebar by its stack index, this finds
     * the corresponding item in the tree using the stack index stored in the
     * item's data and sets it as the current item in the tree, which will
     * trigger the itemClicked signal and navigate to the corresponding section
     *
     * @param stackIndex The stack index of the section to select, used to find
     * the corresponding item in the tree
     */
    void SettingsSidebar::selectByStackIndex(int stackIndex)
    {
        auto* item = _findByStackIndex(stackIndex);
        if (item == nullptr)
            return;

        _tree->setCurrentItem(item);
    }

    /**
     * @brief Set the callback function to be called when a section is selected
     * in the sidebar, this allows the sidebar to communicate with the parent
     * dialog to navigate to the appropriate section when an item is clicked
     *
     * @param callback The callback function to call when a section is selected,
     * it should take an int parameter which is the stack index of the selected
     * section
     */
    void SettingsSidebar::setOnSectionSelected(OnSectionSelected callback)
    {
        _onSectionSelected = std::move(callback);
    }

    /**
     * @brief Get the number of top-level sections in the sidebar, this returns
     * the count of top-level items in the tree widget, which corresponds to
     * the number of main sections (not counting subsections) in the sidebar
     *
     * @return int The number of top-level sections in the sidebar
     */
    int SettingsSidebar::count() const { return _tree->topLevelItemCount(); }

    /**
     * @brief Find a tree widget item by its stack index, this performs a
     * breadth-first search through all items in the tree to find the item that
     * has the given stack index stored in its data, this is used to find the
     * corresponding item when updating dirty state or selecting by stack index
     *
     * @param stackIndex The stack index to search for, used to find the
     * corresponding item in the tree
     * @return QTreeWidgetItem* A pointer to the found item, or nullptr if no
     * item with the given stack index is found
     */
    QTreeWidgetItem* SettingsSidebar::_findByStackIndex(int stackIndex) const
    {
        QList<QTreeWidgetItem*> queue;
        for (int i = 0; i < _tree->topLevelItemCount(); ++i)
            queue.append(_tree->topLevelItem(i));

        while (!queue.isEmpty())
        {
            auto* item = queue.takeFirst();
            if (item->data(0, kStackIndexRole).toInt() == stackIndex)
                return item;

            for (int i = 0; i < item->childCount(); ++i)
                queue.append(item->child(i));
        }

        return nullptr;
    }

    /**
     * @brief Handle item clicked event, this is called when an item in the tree
     * is clicked, it retrieves the stack index from the item's data and calls
     * the _onSectionSelected callback with that index to navigate to the
     * corresponding section
     *
     * @param item The tree widget item that was clicked
     */
    void SettingsSidebar::_onItemClicked(QTreeWidgetItem* item, int /*column*/)
    {
        if (_onSectionSelected)
            _onSectionSelected(item->data(0, kStackIndexRole).toInt());
    }

}   // namespace ui