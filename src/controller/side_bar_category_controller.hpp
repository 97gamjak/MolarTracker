#ifndef __CONTROLLER__SIDE_BAR_CATEGORY_CONTROLLER_HPP__
#define __CONTROLLER__SIDE_BAR_CATEGORY_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class Category;   // Forward declaration

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
        Category* _category;

        /// Pointer to the main window
        QMainWindow* _mainWindow;

       protected:
        [[nodiscard]] QMainWindow* getMainWindow();

       public:
        ~SideBarCategoryController() override = default;
        explicit SideBarCategoryController(
            Category*    category,
            QMainWindow* mainWindow
        );

        [[nodiscard]] Category* getCategory() const;

        /**
         * @brief Refresh the category, this should update the items in the
         * category to reflect the current state of the underlying data, this
         * will be called when the underlying data changes and the category
         * needs to be updated to reflect those changes
         *
         */
        virtual void refresh() = 0;
    };
}   // namespace ui

#endif   // __CONTROLLER__SIDE_BAR_CATEGORY_CONTROLLER_HPP__