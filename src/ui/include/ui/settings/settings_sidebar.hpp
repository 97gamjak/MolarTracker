#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <functional>

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
        QListWidget*      _list{nullptr};
        OnSectionSelected _onSectionSelected;

       public:
        explicit SettingsSidebar(QWidget* parent);

        void addSection(const QString& title);
        void setSectionDirty(int index, bool dirty);
        void selectSection(int index);
        void setOnSectionSelected(OnSectionSelected callback);

       private:
        void _onItemClicked(QListWidgetItem* item);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_SIDEBAR_HPP__