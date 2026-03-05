#include "debug_slots_log_level_delegate.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QStyledItemDelegate>
#include <QWidget>

#include "config/logging_base.hpp"
#include "custom_roles.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{
    DebugSlotsLogLevelDelegate::DebugSlotsLogLevelDelegate(QObject* parent)
        : QStyledItemDelegate(parent)
    {
        _logLevelNames = utils::toQStringList(LogLevelMeta::names);
    }

    QWidget* DebugSlotsLogLevelDelegate::createEditor(
        QWidget* parent,
        const QStyleOptionViewItem& /*option*/,
        const QModelIndex& /*index*/
    ) const
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* combo = new QComboBox(parent);
        combo->addItems(_logLevelNames);
        return combo;
    }

    void DebugSlotsLogLevelDelegate::setEditorData(
        QWidget*           editor,
        const QModelIndex& index
    ) const
    {
        const auto levelText = index.data(Qt::DisplayRole).toString();
        const auto idx       = _logLevelNames.indexOf(levelText);

        auto* combo = static_cast<QComboBox*>(editor);

        if (idx >= 0)
            combo->setCurrentIndex(static_cast<int>(idx));
    }

    void DebugSlotsLogLevelDelegate::setModelData(
        QWidget*            editor,
        QAbstractItemModel* model,
        const QModelIndex&  index
    ) const
    {
        auto* combo = static_cast<QComboBox*>(editor);

        const auto levelText = combo->currentText();

        model->setData(index, levelText, Qt::EditRole);
    }

    void DebugSlotsLogLevelDelegate::updateEditorGeometry(
        QWidget*                    editor,
        const QStyleOptionViewItem& option,
        const QModelIndex& /*index*/
    ) const
    {
        editor->setGeometry(option.rect);
    }

    DebugSlotsApplyToChildrenDelegate::DebugSlotsApplyToChildrenDelegate(
        QObject* parent
    )
        : QStyledItemDelegate(parent)
    {
    }

    void DebugSlotsApplyToChildrenDelegate::paint(
        QPainter*                   painter,
        const QStyleOptionViewItem& option,
        const QModelIndex&          index
    ) const
    {
        if (!index.data(CustomRoles::IsCascadeApply).toBool())
            return;

        QStyleOptionButton button;
        button.rect   = _getButtonRect(option);
        button.text   = "Apply to children";
        button.state |= QStyle::State_Enabled;

        if (option.state & QStyle::State_MouseOver)
            button.state |= QStyle::State_MouseOver;

        QApplication::style()
            ->drawControl(QStyle::CE_PushButton, &button, painter);
    }

    QWidget* DebugSlotsApplyToChildrenDelegate::createEditor(
        QWidget*                    parent,
        const QStyleOptionViewItem& option,
        const QModelIndex&          index
    ) const
    {
        return nullptr;
    }

    bool DebugSlotsApplyToChildrenDelegate::editorEvent(
        QEvent*                     event,
        QAbstractItemModel*         model,
        const QStyleOptionViewItem& option,
        const QModelIndex&          index
    )
    {
        if (!index.data(CustomRoles::IsCascadeApply).toBool())
            return false;

        if (event->type() == QEvent::MouseButtonRelease)
        {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            auto  buttonRect = _getButtonRect(option);

            if (buttonRect.contains(mouseEvent->pos()))
            {
                emit applyToChildrenRequested(index);
                return true;
            }
        }

        return false;
    }

    QRect DebugSlotsApplyToChildrenDelegate::_getButtonRect(
        const QStyleOptionViewItem& option
    ) const
    {
        auto buttonRect = option.rect;
        return buttonRect;
    }

}   // namespace ui