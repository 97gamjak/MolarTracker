#ifndef __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__

#include "category.hpp"

class QAction;

namespace ui
{
    /**
     * @brief Category for managing securities-related UI components.
     *
     * This class provides the necessary methods to handle the securities
     * category within the side bar.
     */
    class SecuritiesCategory : public Category
    {
       private:
        /// Pointer to the "Add Security" action.
        QAction* _createAction;

       public:
        SecuritiesCategory();

        void populateContextMenu(QMenu& menu) override;

        [[nodiscard]] QAction* getCreateAction() const;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__SECURITIES_CATEGORY_HPP__
