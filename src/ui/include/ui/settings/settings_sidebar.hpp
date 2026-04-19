#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__

#include <QVBoxLayout>
#include <QWidget>
#include <functional>

class QTreeWidget;
class QTreeWidgetItem;

namespace ui
{

    /**
     * @brief Left sidebar listing all param container sections.
     *        Each entry shows a dirty indicator dot when that section
     *        has uncommitted (unsaved) params.
     *
     *        Emits sectionSelected via a registered callback rather than
     *        a Qt signal — keeps it QObject-free.
     */
    class SettingsSidebar : public QWidget
    {
       public:
        using OnSectionSelected = std::function<void(int index)>;

       private:
        QTreeWidget*      _tree{nullptr};
        OnSectionSelected _onSectionSelected;

        static constexpr int kStackIndexRole = Qt::UserRole;
        static constexpr int kBaseTitleRole  = Qt::UserRole + 1;

       public:
        explicit SettingsSidebar(QWidget* parent);

        void             addTopLevel(const QString& title, int stackIndex);
        QTreeWidgetItem* addParent(const QString& title, int stackIndex);

        void addChild(
            QTreeWidgetItem* parent,
            const QString&   title,
            int              stackIndex
        );

        void setSectionDirty(int index, bool dirty);
        void selectByStackIndex(int stackIndex);
        void setOnSectionSelected(OnSectionSelected callback);
        void _onItemClicked(QTreeWidgetItem* item, int /*column*/);

        [[nodiscard]] int count() const;

       private:
        [[nodiscard]]
        QTreeWidgetItem* _findByStackIndex(int stackIndex) const;
        void             _onItemClicked(QTreeWidgetItem* item);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__