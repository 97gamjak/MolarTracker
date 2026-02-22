#ifndef __UI__WIDGETS__LOGGING__DEBUG_SLOTS_DIALOG_HPP__
#define __UI__WIDGETS__LOGGING__DEBUG_SLOTS_DIALOG_HPP__

#include <QDialog>

#include "config/logging_base.hpp"

class QCheckBox;          // Forward declaration
class QPushButton;        // Forward declaration
class QTreeWidget;        // Forward declaration
class QDialogButtonBox;   // Forward declaration
class QMainWindow;        // Forward declaration

namespace ui
{

    /**
     * @brief Dialog for managing debug flag categories
     *
     */
    class DebugSlotsDialog final : public QDialog
    {
        Q_OBJECT

       private:
        /// The reference map of log categories
        LogCategoryMap _categories;
        /// The current map of log categories
        LogCategoryMap _currentCategories;

        /// Flag to indicate if only modified categories should be shown in the
        /// tree
        bool _modifiedOnly = false;

        /// UI elements
        QTreeWidget* _tree{};

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
        enum class Action
        {
            Apply,
            ApplyAndClose,
            ResetDefault
        };

        explicit DebugSlotsDialog(QWidget* parent = nullptr);

        void setCategories(const LogCategoryMap& categories);
        void setCategories(
            const LogCategoryMap& categories,
            const bool            overrideReference
        );
        void populateTree();

       signals:
        /// Signal emitted when the user requests to apply changes
        void requested(const Action& action, const LogCategoryMap& categories);

       private:
        void _buildUi();
        void _connectButtons();

        void _emit(const Action& action);
        void _emitApply();
        void _emitApplyAndClose();
        void _emitDefaults();
        void _discardChanges();
        void _rejectChanges();
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_DIALOG_HPP__