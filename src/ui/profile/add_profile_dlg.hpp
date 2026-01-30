#ifndef __UI__PROFILE__ADD_PROFILE_DLG_HPP__
#define __UI__PROFILE__ADD_PROFILE_DLG_HPP__

#include <QDialog>

namespace drafts
{
    struct ProfileDraft;   // Forward declaration
}

namespace app
{
    class ProfileStore;   // Forward declaration
}

namespace settings
{
    class Settings;   // Forward declaration
}

class QLineEdit;     // Forward declaration
class QPushButton;   // Forward declaration
class QCheckBox;     // Forward declaration
class QVBoxLayout;   // Forward declaration

namespace ui
{
    class UndoStack;   // Forward declaration

    /**
     * @brief Dialog for adding a new profile.
     *
     */
    class AddProfileDialog : public QDialog
    {
        Q_OBJECT

       private:
        app::ProfileStore&  _profileStore;
        settings::Settings& _settings;
        UndoStack&          _undoStack;
        bool                _enforceDefaultProfile = false;

        QVBoxLayout* _mainLayout           = nullptr;
        QLineEdit*   _nameLineEdit         = nullptr;
        QLineEdit*   _emailLineEdit        = nullptr;
        QPushButton* _addButton            = nullptr;
        QPushButton* _cancelButton         = nullptr;
        QCheckBox*   _setActiveCheckBox    = nullptr;
        QCheckBox*   _setAsDefaultCheckBox = nullptr;

       protected:
        void accept() override;

       public:
        explicit AddProfileDialog(
            app::ProfileStore&  profileStore,
            settings::Settings& settings,
            UndoStack&          undoStack,
            QWidget*            parent = nullptr
        );

        void setEnforceDefaultProfile(bool value);

       private:
        void _buildUI();
        void _buildFormSection();
        void _buildToggleSection();
        void _buildButtonSection();

        void _updateToggleStates();

        [[nodiscard]] struct drafts::ProfileDraft _getProfile() const;
    };

}   // namespace ui

#endif   // __UI__PROFILE__ADD_PROFILE_DLG_HPP__