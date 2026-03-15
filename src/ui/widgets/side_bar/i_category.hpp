#ifndef __UI__WIDGETS__SIDE_BAR__I_CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__I_CATEGORY_HPP__

#include <QWidget>

namespace ui
{
    class ICategory : public QWidget
    {
        Q_OBJECT

       public:
        explicit ICategory(QWidget* parent);

        [[nodiscard]] virtual QString getName() const = 0;
        virtual void                  refresh()       = 0;

       signals:
        void itemSelected(int id);
        void itemDeleteRequested(int id);
        void itemRenameRequested(int id, const QString& newName);
        void createRequested();
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__I_CATEGORY_HPP__