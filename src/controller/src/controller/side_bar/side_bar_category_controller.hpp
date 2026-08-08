#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CATEGORY_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CATEGORY_CONTROLLER_HPP__

#include <QObject>

#include "side_bar_action.hpp"

class QMainWindow;   // Forward declaration
class QAction;       // Forward declaration

namespace ui
{
    class Category;      // Forward declaration
    class SideBarItem;   // Forward declaration
}   // namespace ui

namespace controller
{
    /**
     * @brief Base controller for a category in the side bar, this is
     * responsible for managing the state of the category and its items, and
     * providing an interface for refreshing the category when the underlying
     * data changes
     *
     */
    class SideBarCategoryController : public QObject
    {
        Q_OBJECT

       private:
        /// The category that this controller manages
        ui::Category* _category;

        /// Pointer to the main window
        QMainWindow* _mainWindow;

       protected:
        [[nodiscard]] QMainWindow* getMainWindow();

       public:
        ~SideBarCategoryController() override = default;

        explicit SideBarCategoryController(
            ui::Category* category,
            QMainWindow*  mainWindow
        );

        [[nodiscard]] ui::Category* getCategory() const;

        /**
         * @brief Refresh the category, this should update the items in the
         * category to reflect the current state of the underlying data, this
         * will be called when the underlying data changes and the category
         * needs to be updated to reflect those changes
         *
         */
        virtual void refresh() = 0;

        void handleTriggeredAction(
            ui::SideBarItem* item,
            SideBarAction    action,
            const QAction*   qaction
        );

       protected:
        virtual void _onItemClicked(ui::SideBarItem* item);

        virtual void _onContextMenuRequested(
            ui::SideBarItem* item,
            const QAction*   action
        );

        virtual void _onItemDoubleClicked(ui::SideBarItem* item);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CATEGORY_CONTROLLER_HPP__
