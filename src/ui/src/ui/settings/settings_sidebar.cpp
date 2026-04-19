#include "ui/settings/settings_sidebar.hpp"

#include <qlistwidget.h>

#include <QListWidgetItem>

#include "utils/qt_helpers.hpp"

namespace ui
{

    SettingsSidebar::SettingsSidebar(QWidget* parent) : QWidget(parent)
    {
        auto* layout = utils::makeQChild<QVBoxLayout>(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _list = utils::makeQChild<QListWidget>(this);
        _list->setObjectName("settingsSidebar");
        _list->setFrameShape(QFrame::NoFrame);
        _list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _list->setSpacing(1);

        connect(
            _list,
            &QListWidget::itemClicked,
            this,
            &SettingsSidebar::_onItemClicked
        );

        layout->addWidget(_list);
    }

    void SettingsSidebar::addSection(const QString& title)
    {
        auto* item = utils::makeQChild<QListWidgetItem>(title, _list);
        item->setData(Qt::UserRole, false);   // dirty = false
        item->setData(
            Qt::UserRole + 1,
            title
        );   // base title for dirty dot suffix
        item->setSizeHint({_list->width(), 36});
    }

    void SettingsSidebar::setSectionDirty(int index, bool dirty)
    {
        if (index < 0 || index >= _list->count())
            return;

        auto* item = _list->item(index);
        item->setData(Qt::UserRole, dirty);

        // Append/remove the dot suffix in the label
        const QString base = item->data(Qt::UserRole + 1).toString();
        item->setText(dirty ? base + "  ●" : base);
    }

    void SettingsSidebar::selectSection(int index)
    {
        if (index < 0 || index >= _list->count())
            return;

        _list->setCurrentRow(index);
    }

    void SettingsSidebar::setOnSectionSelected(OnSectionSelected callback)
    {
        _onSectionSelected = std::move(callback);
    }

    void SettingsSidebar::_onItemClicked(QListWidgetItem* item)
    {
        if (_onSectionSelected)
            _onSectionSelected(_list->row(item));
    }

}   // namespace ui