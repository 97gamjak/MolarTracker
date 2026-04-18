#include "ui/central/central_widget.hpp"

#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("UI.Central.CentralWidget");

namespace ui
{
    /**
     * @brief Construct a new Central Widget:: Central Widget object
     *
     * @param parent
     */
    CentralWidget::CentralWidget(QWidget* parent) : QStackedWidget(parent) {}

}   // namespace ui