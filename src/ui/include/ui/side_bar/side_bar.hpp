#ifndef __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__

#include <QWidget>

class QTreeView;            // Forward declaration
class QStandardItemModel;   // Forward declaration

namespace ui
{
    class Category;      // Forward declaration
    class SideBarItem;   // Forward declaration

    /**
     * @brief The side bar widget, this is responsible for displaying the
     * categories and items in the side bar, and emitting signals when items are
     * clicked or when context menu actions are triggered
     *
     */
    class SideBar : public QWidget
    {
        Q_OBJECT

       private:
        /// The tree view for displaying the categories and items in the side
        /// bar
        QTreeView* _tree;
        /// The model for the tree view, this is used to manage the items in the
        /// side bar
        QStandardItemModel* _model;

       public:
        explicit SideBar(QWidget* parent);

        void addCategory(Category* category);
        void removeCategory(SideBarItem* item);

       signals:
        /// Emitted when an item in the side bar is clicked
        void itemClicked(SideBarItem* item);
        /// Emitted when a context menu action is triggered for an item in the
        /// side bar
        void contextMenuRequested(SideBarItem* item, QAction* action);

       private:
        void _buildUI();

        void _onClicked(const QModelIndex& index);
        void _showContextMenu(const QPoint& pos);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__