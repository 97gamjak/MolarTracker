#include "ui/central/central_widget.hpp"

#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("UI.Central.CentralWidget");

namespace ui
{
    CentralWidget::CentralWidget(QWidget* parent) : QStackedWidget(parent) {}
}   // namespace ui