#ifndef __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__
#define __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__

#include <QDialog>
#include <cstdint>
#include <unordered_map>
#include <vector>

enum class LogCategory : size_t;
enum class LogLevel : size_t;

class QCheckBox;          // Forward declaration
class QPushButton;        // Forward declaration
class QTreeWidget;        // Forward declaration
class QDialogButtonBox;   // Forward declaration

namespace ui
{
    class MainWindow;   // Forward declaration

    class DebugSlotsDialog final : public QDialog
    {
        Q_OBJECT

       private:
        std::unordered_map<LogCategory, LogLevel> _categories;
        std::unordered_map<LogCategory, LogLevel> _currentCategories{};

        bool _modifiedOnly = false;

        QTreeWidget*      _tree{};
        QPushButton*      _defaultsButton{};
        QPushButton*      _discardChangesButton{};
        QCheckBox*        _showOnlyModifiedCheckBox{};
        QDialogButtonBox* _buttonBox{};

       public:
        explicit DebugSlotsDialog(MainWindow& parent);

       private:
        void _buildUi();
        void _populateTree();
        void _connectButtons();
        void _initCategories();
        void _applyChanges();
    };

}   // namespace ui

#endif   // __UI__LOGGING__DEBUG_SLOTS_DIALOG_HPP__