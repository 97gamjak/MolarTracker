#include "overview_category.hpp"

#include <QLabel>
#include <QVBoxLayout>

namespace ui
{
    OverviewCategory::OverviewCategory(QWidget* parent) : ICategory(parent)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QVBoxLayout(this);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        layout->addWidget(new QLabel("Overview coming soon...", this));
    }

    QString OverviewCategory::getName() const { return "Overview"; }

    void OverviewCategory::refresh()
    {
        // Refresh logic for the overview category
    }
}   // namespace ui