#ifndef __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__
#define __UI__PROFILE__PROFILE_SELECTION_DLG_HPP__

#include <QDialog>
#include <QListWidget>
#include <vector>

namespace ui
{
    class ProfileSelectionDialog : public QDialog
    {
        Q_OBJECT

       private:
        QListWidget* _profileListWidget = nullptr;

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