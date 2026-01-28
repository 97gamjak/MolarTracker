#ifndef __UI__PROFILE__ADD_PROFILE_DLG_HPP__
#define __UI__PROFILE__ADD_PROFILE_DLG_HPP__

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

namespace drafts
{
    struct ProfileDraft;
}

namespace app
{
    class ProfileStore;
}   // namespace app

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

        app::ProfileStore& _profileStore;
        bool               _setAsActive = false;

       protected:
        void accept() override;

       public:
        explicit AddProfileDialog(
            app::ProfileStore& profileStore,
            QWidget*           parent = nullptr
        );

        void setAsActive(bool value) { _setAsActive = value; }

        [[nodiscard]] struct drafts::ProfileDraft getProfile() const;

       private:
        void _buildUI();
    };

}   // namespace ui

#endif   // __UI__PROFILE__ADD_PROFILE_DLG_HPP__