#include "ui/settings/param_editor.hpp"

namespace ui
{
    void onDirtyStripe(void* userData, const bool& isDirty)
    {
        auto* stripe = static_cast<QWidget*>(userData);
        stripe->setProperty("dirty", isDirty);
        stripe->style()->unpolish(stripe);
        stripe->style()->polish(stripe);
    }
}   // namespace ui