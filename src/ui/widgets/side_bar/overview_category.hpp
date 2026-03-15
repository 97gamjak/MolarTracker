#ifndef __UI__WIDGETS__SIDE_BAR__OVERVIEW_CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__OVERVIEW_CATEGORY_HPP__

#include "i_category.hpp"

namespace ui
{
    class OverviewCategory : public ICategory
    {
        Q_OBJECT

       public:
        explicit OverviewCategory(QWidget* parent);

        [[nodiscard]] QString getName() const override;
        void                  refresh() override;
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__OVERVIEW_CATEGORY_HPP__