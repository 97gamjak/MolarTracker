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

       private:
        void _buildUI(std::vector<std::string> profiles);
    };

}   // namespace ui

#endif   // __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__