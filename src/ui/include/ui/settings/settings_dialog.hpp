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
        /// Fixed width for the sidebar
        static constexpr size_t _sideBarWidth = 160;

        /// Reference to the application settings
        settings::Settings& _settings;

        /// Pointer to the sidebar widget
        SettingsSidebar* _sidebar{nullptr};
        /// Pointer to the stacked widget that holds the settings sections
        QStackedWidget* _stack{nullptr};
        /// Pointer to the label that indicates if there are unsaved changes
        QLabel* _unsavedLabel{nullptr};

        /// Connection lifetimes for all dirty subscriptions
        std::vector<Connection> _connections;

        /// forward declaration of the dirty callback context struct
        struct DirtyContext;
        /// Heap-allocated dirty callback contexts — one per section.
        /// Freed in destructor after _connections are disconnected.
        std::vector<std::unique_ptr<DirtyContext>> _dirtyContexts;

       public:
        explicit SettingsDialog(settings::Settings& settings, QWidget* parent);

       signals:
        /**
         * @brief signal emitted when the user clicks the Save button in the
         * settings dialog
         *
         */
        void saveRequested();

       private:
        void        _build();
        void        _applyStyleSheet();
        void        _updateUnsavedLabel();
        static void _onSectionDirty(void* userData, const bool& isDirty);
    };

    /**
     * @brief Context struct for dirty state callbacks, holds a pointer to the
     * dialog and the index of the section in the sidebar/stack. This allows the
     * callback to update the correct section's dirty indicator and the overall
     * unsaved label when any param in that section changes.
     */
    struct SettingsDialog::DirtyContext
    {
        /// Pointer back to the dialog, used to call member functions to update
        /// the UI when dirty state changes
        SettingsDialog* dialog;

        /// The index of the section in the sidebar/stack that this context is
        /// associated with, used to update the correct dirty indicator in the
        /// sidebar when dirty state changes
        int index;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_DIALOG_HPP__