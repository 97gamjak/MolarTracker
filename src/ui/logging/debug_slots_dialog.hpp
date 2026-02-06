#ifndef __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__
#define __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__

#include <QDialog>
#include <unordered_map>

#include "logging/logging_base.hpp"

class QCheckBox;          // Forward declaration
class QPushButton;        // Forward declaration
class QTreeWidget;        // Forward declaration
class QDialogButtonBox;   // Forward declaration
class QMainWindow;        // Forward declaration

namespace ui
{

    class DebugSlotsDialog final : public QDialog
    {
        Q_OBJECT

       private:
        LogCategoryMap _categories;
        LogCategoryMap _currentCategories;

        bool _modifiedOnly = false;

        QTreeWidget*      _tree{};
        QPushButton*      _defaultsButton{};
        QPushButton*      _discardChangesButton{};
        QCheckBox*        _showOnlyModifiedCheckBox{};
        QDialogButtonBox* _buttonBox{};

       public:
        enum class Action
        {
            Apply,
            ApplyAndClose,
            ResetDefault,
        };

        explicit DebugSlotsDialog(QWidget* parent = nullptr);

        void setCategories(const LogCategoryMap& categories);
        void populateTree();

       signals:
        void requested(const Action& action, const LogCategoryMap& categories);

       private:
        void _buildUi();
        void _connectButtons();
        void _initCategories();
        void _applyChanges();

        void _emit(const Action& action);
        void _emitApply();
        void _emitDefaults();
    };

}   // namespace ui

#endif   // __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__