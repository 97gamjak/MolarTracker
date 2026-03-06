#ifndef __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__
#define __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__

#include <QStyledItemDelegate>

namespace ui
{
    /**
     * @brief A delegate for the log level column in the debug slots tree view.
     * It provides a combo box editor to select the log level for each slot.
     *
     */
    class DebugSlotsLogLevelDelegate final : public QStyledItemDelegate
    {
        Q_OBJECT

       private:
        /// A list of log level names corresponding to the LogLevel enum values
        QStringList _logLevelNames;

       public:
        explicit DebugSlotsLogLevelDelegate(QObject* parent);

        QWidget* createEditor(
            QWidget*                    parent,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
        ) const override;

        void setEditorData(
            QWidget*           editor,
            const QModelIndex& index
        ) const override;

        void setModelData(
            QWidget*            editor,
            QAbstractItemModel* model,
            const QModelIndex&  index
        ) const override;

        void updateEditorGeometry(
            QWidget*                    editor,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
        ) const override;
    };

    /**
     * @brief A delegate that adds an "Apply to Children" button to the debug
     * slots tree view. When the button is clicked, it emits a signal with the
     * index of the item for which the action should be applied to its children.
     *
     */
    class DebugSlotsApplyToChildrenDelegate final : public QStyledItemDelegate
    {
        Q_OBJECT

       public:
        explicit DebugSlotsApplyToChildrenDelegate(QObject* parent);

        void paint(
            QPainter*                   painter,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
        ) const override;

        QWidget* createEditor(
            QWidget*                    parent,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
        ) const override;

        bool editorEvent(
            QEvent*                     event,
            QAbstractItemModel*         model,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
        ) override;

       signals:
        /// Emitted when the "Apply to Children" button is clicked, with the
        /// index of the item for which the action should be applied to its
        /// children
        void applyToChildrenRequested(const QModelIndex& index) const;

       private:
        [[nodiscard]] QRect _getButtonRect(
            const QStyleOptionViewItem& option
        ) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__