#ifndef __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__
#define __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__

#include <QStyledItemDelegate>

namespace ui
{
    class DebugSlotsLogLevelDelegate final : public QStyledItemDelegate
    {
        Q_OBJECT

       private:
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
        void applyToChildrenRequested(const QModelIndex& index) const;

       private:
        [[nodiscard]] QRect _getButtonRect(
            const QStyleOptionViewItem& option
        ) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_LOG_LEVEL_DELEGATE_HPP__