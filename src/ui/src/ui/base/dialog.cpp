#include "ui/base/dialog.hpp"

#include "config/constants.hpp"

namespace ui
{
    Dialog::Dialog(QWidget* parent) : QDialog(parent)
    {
        const auto [x, y] = Constants::getGlobalMinDialogSize();
        setMinimumSize(x, y);
    }
}   // namespace ui
