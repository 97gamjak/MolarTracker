#include "securities_controller.hpp"

#include "ui/side_bar/securities_category.hpp"

namespace controller
{
    SecuritiesSideBarController::SecuritiesSideBarController(
        QMainWindow* mainWindow
    )
        : SideBarCategoryController(new ui::SecuritiesCategory(), mainWindow)
    {
    }

    void SecuritiesSideBarController::refresh() {}

}   // namespace controller
