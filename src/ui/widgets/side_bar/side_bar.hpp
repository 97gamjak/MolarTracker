#ifndef __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__
#define __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__

#include <QWidget>

class QTreeView;            // Forward declaration
class QStandardItemModel;   // Forward declaration

namespace ui
{
    class Category;      // Forward declaration
    class SideBarItem;   // Forward declaration

    class SideBar : public QWidget
    {
        Q_OBJECT

       private:
        QTreeView*          _tree;
        QStandardItemModel* _model;

       public:
        explicit SideBar(QWidget* parent);

        void addCategory(Category* category);
        void removeCategory(SideBarItem* item);

       signals:
        void itemClicked(SideBarItem* item);
        void contextMenuRequested(SideBarItem* item, QAction* action);

       private:
        void _buildUI();

        void _onClicked(const QModelIndex& index);
        void _showContextMenu(const QPoint& pos);
    };

}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__