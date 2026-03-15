#include "account_category.hpp"

#include <QListWidget>
#include <QVBoxLayout>

namespace ui
{
    AccountCategory::AccountCategory(QWidget* parent)
        : ICategory(parent), _accountList(new QListWidget(this))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        _accountList->setContextMenuPolicy(Qt::CustomContextMenu);
        layout->addWidget(_accountList);

        connect(
            _accountList,
            &QListWidget::itemDoubleClicked,
            this,
            &AccountCategory::onItemDoubleClicked
        );

        _accountList->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(
            _accountList,
            &QListWidget::customContextMenuRequested,
            this,
            &AccountCategory::showContextMenu
        );
    }

    QString AccountCategory::getName() const { return "Accounts"; }

    void AccountCategory::refresh()
    {
        _accountList->clear();
        // TODO(97gamjak): Load accounts from the profile and populate the list
        auto* dummyWidget = new QListWidgetItem("Dummy Widget");
        dummyWidget->setData(Qt::UserRole, 1);
        _accountList->addItem(dummyWidget);
    }

    void AccountCategory::onItemDoubleClicked(QListWidgetItem* item)
    {
        const auto id = item->data(Qt::UserRole).toInt();
        emit       itemSelected(id);
    }

    void AccountCategory::showContextMenu(const QPoint& pos)
    {
        auto* item = _accountList->itemAt(pos);
        if (item)
        {
            // TODO(97gamjak): Implement context menu for account items
        }
        else
        {
            // TODO(97gamjak): Implement context menu for empty space (e.g. add
            // new account)
        }
    }

}   // namespace ui