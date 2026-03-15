#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__

#include "i_category.hpp"

class QListWidget;       // Forward declaration
class QListWidgetItem;   // Forward declaration

namespace ui
{
    class AccountCategory : public ICategory
    {
        Q_OBJECT

       private:
        QListWidget* _accountList;

       public:
        explicit AccountCategory(QWidget* parent);

        [[nodiscard]] QString getName() const override;
        void                  refresh() override;

       private:
        void onItemDoubleClicked(QListWidgetItem* item);
        void showContextMenu(const QPoint& pos);
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__