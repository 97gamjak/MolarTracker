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
        /// Type alias for the section selected callback function
        using OnSectionSelected = std::function<void(int index)>;

       private:
        /// Pointer to the tree widget that displays the sections in the sidebar
        QTreeWidget* _tree{nullptr};
        /// Callback function that is called when a section is selected
        OnSectionSelected _onSectionSelected;

        /// Custom data role for storing the stack index
        static constexpr int kStackIndexRole = Qt::UserRole;
        /// Custom data role for storing the base title (without dirty dot)
        static constexpr int kBaseTitleRole = Qt::UserRole + 1;

       public:
        explicit SettingsSidebar(QWidget* parent);

        void             addTopLevel(const QString& title, int stackIndex);
        QTreeWidgetItem* addParent(const QString& title, int stackIndex);

        static void addChild(
            QTreeWidgetItem* parent,
            const QString&   title,
            int              stackIndex
        );

        void setSectionDirty(int index, bool dirty);
        void selectByStackIndex(int stackIndex);
        void setOnSectionSelected(OnSectionSelected callback);

        [[nodiscard]] int count() const;

       private:
        [[nodiscard]]
        QTreeWidgetItem* _findByStackIndex(int stackIndex) const;
        void             _onItemClicked(QTreeWidgetItem* item, int /*column*/);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__