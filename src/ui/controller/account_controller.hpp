#ifndef __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include "side_bar_category_controller.hpp"

class QAction;   // Forward declaration

namespace ui
{
    class AccountCategory;   // Forward declaration

    /**
     * @brief Controller for the account category in the side bar
     *
     */
    class AccountSideBarController : public SideBarCategoryController
    {
       public:
        explicit AccountSideBarController();

        void refresh() override;

        static void handleContextMenuAction(
            const AccountCategory* item,
            const QAction*         action
        );
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__