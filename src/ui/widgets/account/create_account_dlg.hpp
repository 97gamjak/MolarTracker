#ifndef __UI__WIDGETS__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__
#define __UI__WIDGETS__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__

#include <QDialog>

class QVBoxLayout;   // Forward declaration
class QComboBox;     // Forward declaration

namespace drafts
{
    struct AccountDraft;   // Forward declaration
}

namespace ui
{
    class NameLineEdit;   // Forward declaration

    class CreateAccountDialog : public QDialog
    {
        Q_OBJECT

       private:
        /// Line edit for the profile name
        NameLineEdit* _nameLineEdit = nullptr;

        QComboBox* _accountType = nullptr;

        QComboBox* _currency = nullptr;

        QPushButton* _addButton    = nullptr;
        QPushButton* _cancelButton = nullptr;

       public:
        explicit CreateAccountDialog(QWidget* parent);

       signals:
        /**
         * @brief QT signal for when an action is performed in the create
         * account dlg
         *
         * @param account
         */
        void requested(const drafts::AccountDraft& account);

       private:
        void _buildUI();

        void _buildFormLayout(QVBoxLayout* parent);
        void _buildButtonSection(QVBoxLayout* parent);

        void _emitOk();

        drafts::AccountDraft _getAccount();
    };

}   // namespace ui

#endif   // __UI__WIDGETS__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__