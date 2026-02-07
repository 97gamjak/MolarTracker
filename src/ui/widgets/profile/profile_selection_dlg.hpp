#ifndef __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__
#define __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__

#include <QDialog>
#include <string>
#include <vector>

class QWidget;            // Forward declaration
class QListWidget;        // Forward declaration
class QDialogButtonBox;   // Forward declaration

namespace ui
{
    /**
     * @brief Dialog for selecting a profile from existing profiles.
     *
     */
    class ProfileSelectionDialog : public QDialog
    {
        Q_OBJECT

       private:
        QListWidget*      _profileListWidget = nullptr;
        QDialogButtonBox* _buttonBox         = nullptr;

       public:
        explicit ProfileSelectionDialog(
            QWidget*                 parent,
            std::vector<std::string> profiles
        );

        enum class Action
        {
            Ok,
            Cancel
        };

       signals:
        void requested(const Action& action, const std::string& profileName);

       private:
        void _buildUI(std::vector<std::string> profiles);

        void _emit(const Action& action, const std::string& profileName);
        void _emit(const Action& action);
        void _emitOk();
        void _emitCancel();
    };

}   // namespace ui

#endif   // __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__