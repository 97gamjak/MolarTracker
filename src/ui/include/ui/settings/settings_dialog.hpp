#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_DIALOG_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_DIALOG_HPP__

#include <QDialog>

#include "connections/connection.hpp"

namespace settings
{
    class Settings;   // Forward declaration
}   //  namespace settings

class QLabel;           // Forward declaration
class QStackedWidget;   // Forward declaration

namespace ui
{

    class SettingsSidebar;   // Forward declaration

    /**
     * @brief Top-level settings dialog.
     *
     *        Layout:
     *          ┌─────────────┬───────────────────────────────┐
     *          │  Sidebar    │  SettingsSectionWidget<T>     │
     *          │             │  (swapped via QStackedWidget) │
     *          ├─────────────┴───────────────────────────────┤
     *          │  [unsaved indicator]          [Save] [Close]│
     *          └─────────────────────────────────────────────┘
     *
     *        Auto-commits param values on edit. Save writes to disk.
     *        Dirty dots in the sidebar update live via subscribeToDirty.
     */
    class SettingsDialog : public QDialog
    {
        Q_OBJECT

       private:
        static constexpr int kGeneralIndex = 0;
        static constexpr int kUiIndex      = 1;
        static constexpr int kLoggingIndex = 2;

        settings::Settings& _settings;

        SettingsSidebar* _sidebar{nullptr};
        QStackedWidget*  _stack{nullptr};
        QLabel*          _unsavedLabel{nullptr};

        // Connection lifetimes for all dirty subscriptions
        std::vector<Connection> _connections;

        // Heap-allocated dirty callback contexts — one per section.
        // Freed in destructor after _connections are disconnected.
        struct DirtyContext
        {
            SettingsDialog* dialog;
            int             index;
        };
        std::vector<std::unique_ptr<DirtyContext>> _dirtyContexts;

       public:
        explicit SettingsDialog(settings::Settings& settings, QWidget* parent);

       signals:
        void saveRequested();

       private:
        void        _build();
        void        _applyStyleSheet();
        void        _updateUnsavedLabel();
        static void _onSectionDirty(void* userData, const bool& isDirty);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_DIALOG_HPP__