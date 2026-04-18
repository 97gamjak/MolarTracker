#ifndef __UI__INCLUDE__UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__
#define __UI__INCLUDE__UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__

#include "debug_slots_model.hpp"
#include "ui/base/dialog.hpp"

class QCheckBox;          // Forward declaration
class QPushButton;        // Forward declaration
class QTreeView;          // Forward declaration
class QDialogButtonBox;   // Forward declaration
class QMainWindow;        // Forward declaration

namespace ui
{

    /**
     * @brief Dialog for managing debug flag categories
     *
     */
    class DebugSlotsDialog final : public Dialog
    {
        Q_OBJECT

       public:
        struct Settings;

       private:
        /// Settings for the debug slots dialog
        std::shared_ptr<Settings> _settings;

        /// The reference map of log categories
        logging::LogCategories _categories;
        /// The current map of log categories
        logging::LogCategories _currentCategories;

        /// Flag to indicate if only modified categories should be shown in the
        /// tree
        bool _modifiedOnly = false;

        /// UI elements
        QTreeView* _tree{};

        /// The model for the tree view of the LogCategories
        LogCategoryModel* _model{};

        /// Buttons
        QPushButton* _defaultsButton{};

        /// Button to apply changes without closing the dialog
        QPushButton* _discardChangesButton{};

        /// Checkbox to show only modified categories
        QCheckBox* _showOnlyModifiedCheckBox{};

        /// Button box to hold the buttons
        QDialogButtonBox* _buttonBox{};

       public:
        /**
         * @brief Action enum for the debug slots dialog
         *
         */
        enum class Action : std::uint8_t
        {
            Apply,
            ApplyAndClose,
            ResetDefault
        };

        explicit DebugSlotsDialog(
            std::shared_ptr<Settings> settings,
            QWidget*                  parent
        );

        void setCategories(const logging::LogCategories& categories);
        void setCategories(
            const logging::LogCategories& categories,
            bool                          overrideReference
        );
        void populateTree();

       signals:
        /// Signal emitted when the user requests to apply changes
        void requested(
            const Action&                 action,
            const logging::LogCategories& categories
        );

       private:
        void _buildUi();
        void _connectButtons();

        void _emit(const Action& action);
        void _emitApply();
        void _emitApplyAndClose();
        void _emitDefaults();
        void _discardChanges();
        void _rejectChanges();
        void _applyToChildren(const QModelIndex& idx);
    };

    /**
     * @brief Settings for DebugSlotsDialog
     *
     */
    struct DebugSlotsDialog::Settings
    {
       public:
        /// The size of the debug slots dialog window in pixels, represented as
        /// a tuple of (width, height)
        std::pair<int, int> windowSize;

       public:
        Settings() = delete;
        explicit Settings(std::pair<int, int> _windowSize);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__