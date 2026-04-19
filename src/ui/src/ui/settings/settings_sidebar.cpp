#include "ui/settings/settings_sidebar.hpp"

#include <QTreeWidget>

namespace ui
{

    SettingsSidebar::SettingsSidebar(QWidget* parent) : QWidget(parent)
    {
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _tree = new QTreeWidget(this);
        _tree->setObjectName("settingsSidebar");
        _tree->setHeaderHidden(true);
        _tree->setFrameShape(QFrame::NoFrame);
        _tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _tree->setIndentation(12);
        _tree->setAnimated(true);

        connect(
            _tree,
            &QTreeWidget::itemClicked,
            [this](QTreeWidgetItem* item, int column)
            { _onItemClicked(item, column); }
        );

        layout->addWidget(_tree);
    }

    void SettingsSidebar::addTopLevel(const QString& title, int stackIndex)
    {
        auto* item = new QTreeWidgetItem(_tree);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
    }

    QTreeWidgetItem* SettingsSidebar::addParent(
        const QString& title,
        int            stackIndex
    )
    {
        auto* item = new QTreeWidgetItem(_tree);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
        item->setExpanded(true);
        return item;
    }

    void SettingsSidebar::addChild(
        QTreeWidgetItem* parent,
        const QString&   title,
        int              stackIndex
    )
    {
        auto* item = new QTreeWidgetItem(parent);
        item->setText(0, title);
        item->setData(0, kStackIndexRole, stackIndex);
        item->setData(0, kBaseTitleRole, title);
        item->setFlags(
            item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled
        );
    }

    void SettingsSidebar::setSectionDirty(int stackIndex, bool dirty)
    {
        auto* item = _findByStackIndex(stackIndex);
        if (item == nullptr)
            return;

        const QString base = item->data(0, kBaseTitleRole).toString();
        item->setText(0, dirty ? base + "  ●" : base);
    }

    void SettingsSidebar::selectByStackIndex(int stackIndex)
    {
        auto* item = _findByStackIndex(stackIndex);
        if (item == nullptr)
            return;

        _tree->setCurrentItem(item);
    }

    void SettingsSidebar::setOnSectionSelected(OnSectionSelected callback)
    {
        _onSectionSelected = std::move(callback);
    }

    int SettingsSidebar::count() const { return _tree->topLevelItemCount(); }

    QTreeWidgetItem* SettingsSidebar::_findByStackIndex(int stackIndex) const
    {
        // BFS over all items
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

    void SettingsSidebar::_onItemClicked(QTreeWidgetItem* item, int /*column*/)
    {
        if (_onSectionSelected)
            _onSectionSelected(item->data(0, kStackIndexRole).toInt());
    }

}   // namespace ui