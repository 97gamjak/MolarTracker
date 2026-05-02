#ifndef __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__

#include "category.hpp"

namespace ui
{
    class SecuritiesCategory : public Category
    {
       public:
        SecuritiesCategory();

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
