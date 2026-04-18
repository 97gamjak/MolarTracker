#include "ui/base/dialog.hpp"

#include "config/constants.hpp"

namespace ui
{
    /**
     * @brief Construct a new Dialog:: Dialog object
     *
     * @param parent
     */
    Dialog::Dialog(QWidget* parent) : QDialog(parent)
    {
        const auto [x, y] = Constants::getGlobalMinDialogSize();
        setMinimumSize(x, y);
    }
}   // namespace ui
