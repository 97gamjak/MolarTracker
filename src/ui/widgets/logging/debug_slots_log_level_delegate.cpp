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
    /**
     * @brief Construct a new Debug Slots Log Level Delegate:: Debug Slots Log
     * Level Delegate object
     *
     * @param parent The parent QObject
     */
    DebugSlotsLogLevelDelegate::DebugSlotsLogLevelDelegate(QObject* parent)
        : QStyledItemDelegate(parent),
          _logLevelNames(utils::toQStringList(LogLevelMeta::names))
    {
    }

    /**
     * @brief Create an editor widget for the log level column, which is a combo
     * box with the available log levels
     *
     * @param parent The parent widget for the editor
     * @param option The style options for the item being edited
     * @param index The model index of the item being edited
     * @return QWidget* A pointer to the created editor widget
     */
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

    /**
     * @brief Set the data for the editor widget based on the model index
     *
     * @param editor The editor widget to set the data for
     * @param index The model index containing the data to set in the editor
     */
    void DebugSlotsLogLevelDelegate::setEditorData(
        QWidget*           editor,
        const QModelIndex& index
    ) const
    {
        const auto levelText = index.data(Qt::DisplayRole).toString();
        const auto idx       = _logLevelNames.indexOf(levelText);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        auto* combo = static_cast<QComboBox*>(editor);

        if (idx >= 0)
            combo->setCurrentIndex(static_cast<int>(idx));
    }

    /**
     * @brief Set the data in the model based on the editor widget's current
     * value
     *
     * @param editor The editor widget containing the new value to set in the
     * model
     * @param model The model to update with the new value from the editor
     * @param index The model index to update with the new value
     */
    void DebugSlotsLogLevelDelegate::setModelData(
        QWidget*            editor,
        QAbstractItemModel* model,
        const QModelIndex&  index
    ) const
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        auto* combo = static_cast<QComboBox*>(editor);

        const auto levelText = combo->currentText();

        model->setData(index, levelText, Qt::EditRole);
    }

    /**
     * @brief Update the geometry of the editor widget to match the item being
     * edited
     *
     * @param editor The editor widget to update the geometry for
     * @param option The style options for the item being edited, containing the
     * geometry information
     * @param index The model index of the item being edited
     */
    void DebugSlotsLogLevelDelegate::updateEditorGeometry(
        QWidget*                    editor,
        const QStyleOptionViewItem& option,
        const QModelIndex& /*index*/
    ) const
    {
        editor->setGeometry(option.rect);
    }

    /**
     * @brief Construct a new Debug Slots Apply To Children Delegate:: Debug
     * Slots Apply To Children Delegate object
     *
     * @param parent The parent QObject
     */
    DebugSlotsApplyToChildrenDelegate::DebugSlotsApplyToChildrenDelegate(
        QObject* parent
    )
        : QStyledItemDelegate(parent)
    {
    }

    /**
     * @brief Paint the "Apply to Children" button in the item view if the
     * corresponding data role is set to true for the item
     *
     * @param painter The QPainter object used for drawing the button
     * @param option The style options for the item being painted, containing
     * the geometry information
     * @param index The model index of the item being painted, used to check if
     * the button should be displayed based on the custom data role
     */
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

        if ((option.state & QStyle::State_MouseOver) != 0)
            button.state |= QStyle::State_MouseOver;

        QApplication::style()
            ->drawControl(QStyle::CE_PushButton, &button, painter);
    }

    /**
     * @brief Create an editor widget for the "Apply to Children" button, which
     * is not editable and only serves as a clickable area for the button
     *
     * @param parent The parent widget for the editor
     * @param option The style options for the item being edited
     * @param index The model index of the item being edited
     * @return QWidget* A pointer to the created editor widget, which is null
     * since this delegate does not use an actual editor widget
     */
    QWidget* DebugSlotsApplyToChildrenDelegate::createEditor(
        QWidget* /*parent*/,
        const QStyleOptionViewItem& /*option*/,
        const QModelIndex& /*index*/
    ) const
    {
        return nullptr;
    }

    /**
     * @brief Handle mouse events for the "Apply to Children" button, emitting a
     * signal when the button is clicked
     *
     * @param event The event to handle, expected to be a mouse button release
     * event
     * @param model The model associated with the item being interacted with
     * @param option The style options for the item being interacted with,
     * containing the geometry information for the button
     * @param index The model index of the item being interacted with, used to
     * check if the button should be active and to emit the signal with the
     * correct index
     * @return bool True if the event was handled (button click), false
     * otherwise
     */
    bool DebugSlotsApplyToChildrenDelegate::editorEvent(
        QEvent* event,
        QAbstractItemModel* /*model*/,
        const QStyleOptionViewItem& option,
        const QModelIndex&          index
    )
    {
        if (!index.data(CustomRoles::IsCascadeApply).toBool())
            return false;

        if (event->type() == QEvent::MouseButtonRelease)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
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

    /**
     * @brief Get the rectangle area for the "Apply to Children" button based
     * on the item view's style options
     *
     * @param option The style options for the item being painted, containing
     * the geometry information for the button
     * @return QRect The rectangle area where the "Apply to Children" button is
     * drawn and can be interacted with
     */
    QRect DebugSlotsApplyToChildrenDelegate::_getButtonRect(
        const QStyleOptionViewItem& option
    )
    {
        auto buttonRect = option.rect;
        return buttonRect;
    }

}   // namespace ui