#include "side_bar.hpp"

#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace ui
{
    /**
     * @brief Construct a new Side Bar:: Side Bar object
     *
     * @param parent
     */
    SideBar::SideBar(QWidget* parent)
        : QWidget(parent),
          _categoryBar(new QListWidget{}),
          _stack(new QStackedWidget{})
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _categoryBar->setFixedWidth(120);
        _categoryBar->setFlow(QListView::TopToBottom);

        layout->addWidget(_categoryBar);

        connect(
            _categoryBar,
            &QListWidget::currentRowChanged,
            _stack,
            &QStackedWidget::setCurrentIndex
        );
    }

    /**
     * @brief Register a category to the side bar, this will add the category
     * to the category list and the stacked widget, and connect the item
     * selected signal of the category to the item selected signal of the side
     * bar
     *
     * @param category The category to register
     */
    void SideBar::registerCategory(ICategory* category)
    {
        _categoryBar->addItem(category->getName());
        _stack->addWidget(category);
        _categories.push_back(category);

        connect(
            category,
            &ICategory::itemSelected,
            this,
            &SideBar::itemSelected
        );
    }
}   // namespace ui