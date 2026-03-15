#ifndef __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__
#define __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__

#include <vector>

#include "i_category.hpp"

class QListWidget;      // Forward declaration
class QStackedWidget;   // Forward declaration

namespace ui
{
    class SideBar : public QWidget
    {
        Q_OBJECT

       private:
        QListWidget*            _categoryBar;
        QStackedWidget*         _stack;
        std::vector<ICategory*> _categories;

       public:
        explicit SideBar(QWidget* parent);
        void registerCategory(ICategory* category);

       signals:
        void itemSelected(int id);
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__SIDE_BAR_HPP__