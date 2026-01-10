#ifndef __UI__PROFILE__ADD_PROFILE_DLG_HPP__
#define __UI__PROFILE__ADD_PROFILE_DLG_HPP__

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

namespace drafts
{
    struct ProfileDraft;
}

namespace ui
{
    class AddProfileDialog : public QDialog
    {
        Q_OBJECT

       private:
        QLineEdit*   _nameLineEdit     = nullptr;
        QLineEdit*   _emailLineEdit    = nullptr;
        QPushButton* _addProfileButton = nullptr;
        QPushButton* _cancelButton     = nullptr;

       public:
        explicit AddProfileDialog(QWidget* parent = nullptr);

        [[nodiscard]] struct drafts::ProfileDraft getProfile() const;

       private:
        void _buildUI();
    };

}   // namespace ui

#endif   // __UI__PROFILE__ADD_PROFILE_DLG_HPP__