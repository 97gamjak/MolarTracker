#ifndef __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__

#include <QWidget>
#include <mstd/enum.hpp>

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

        /// The old text of the item being renamed, used to restore the text if
        /// the rename is cancelled
        QString _pendingRenameOldText;

       public:
        explicit SideBar(QWidget* parent);

        void addCategory(Category* category);
        void removeCategory(SideBarItem* item);
        void selectItem(SideBarItem* item);

       signals:
        void itemClicked(SideBarItem* item);
        void itemDoubleClicked(SideBarItem* item);
        void contextMenuRequested(SideBarItem* item, QAction* action);
        void itemRenameCommitted(SideBarItem* item, const QString& newName);

       private:
        void _buildUI();

        void _onClicked(const QModelIndex& index);
        void _onDoubleClicked(const QModelIndex& index);
        void _showContextMenu(const QPoint& pos);
        void _onDataChanged(
            const QModelIndex& topLeft,
            const QModelIndex& bottomRight
        );
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SIDE_BAR_HPP__
