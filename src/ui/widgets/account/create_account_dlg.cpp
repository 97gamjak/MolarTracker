#include "create_account_dlg.hpp"

#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

#include "config/finance_enums.hpp"
#include "drafts/account_draft.hpp"
#include "ui/widgets/validators/name_line_edit.hpp"

namespace ui
{
    /**
     * @brief Construct a new Create Account Dialog:: Create Account Dialog
     * object
     *
     * @param parent The parent widget of the dialog
     */
    CreateAccountDialog::CreateAccountDialog(QWidget* parent) : QDialog(parent)
    {
        setWindowTitle("Create Account");

        _buildUI();
    }

    /**
     * @brief Build the UI of the create account dialog
     *
     */
    void CreateAccountDialog::_buildUI()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* mainLayout = new QVBoxLayout{this};

        _buildFormLayout(mainLayout);
        _buildButtonSection(mainLayout);
    }

    /**
     * @brief Build the form layout for creating an account
     *
     * @param parent The parent layout of the form section
     */
    void CreateAccountDialog::_buildFormLayout(QVBoxLayout* parent)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* formLayout = new QFormLayout{};

        parent->addLayout(formLayout);

        // "Credit Card" NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _accountType = new QComboBox{this};
        for (const auto& type : AccountKindMeta::values)
        {
            // we disallow creating accounts of kind External, since those are
            // meant to represent accounts that exist outside of the application
            // and are not meant to be created by the user
            if (type == AccountKind::External)
                continue;

            _accountType->addItem(
                QString::fromStdString(AccountKindMeta::toString(type))
            );
        }
        formLayout->addRow("Account Type:", _accountType);

        auto [nameLineEdit, nameContainer] = createNameLineEditWithLabel(this);

        _nameLineEdit = nameLineEdit;
        _nameLineEdit->setRequired(true);
        _nameLineEdit->allowWhiteSpaces(true);

        formLayout->addRow("Account Name:", nameContainer);

        // "Credit Card" NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _currency = new QComboBox{this};
        for (const auto& type : CurrencyMeta::names)
        {
            _currency->addItem(QString::fromStdString(std::string(type)));
        }
        formLayout->addRow("Currency:", _currency);
    }

    /**
     * @brief build the button section
     *
     * @param parent The parent layout of the button section
     */
    void CreateAccountDialog::_buildButtonSection(QVBoxLayout* parent)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* buttonLayout = new QHBoxLayout{};

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _addButton = new QPushButton{"Create Account", this};

        // check the validity of the input to enable or disable the add button
        _addButton->setEnabled(false);
        connect(
            _nameLineEdit,
            &NameLineEdit::validityChanged,
            _addButton,
            &QPushButton::setEnabled
        );

        // connect the add button to emit the Ok action with the profile draft
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &CreateAccountDialog::_emitOk
        );

        buttonLayout->addWidget(_addButton);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _cancelButton = new QPushButton{"Cancel", this};
        buttonLayout->addWidget(_cancelButton);

        parent->addLayout(buttonLayout);
    }

    /**
     * @brief Emit the requested signal with the account draft created from the
     * user input, this will be called when the add button is clicked, and will
     * take the user input from the form, create an account draft, and emit the
     * requested signal with the draft, allowing the controller to handle the
     * creation of a new account based on the user's input.
     *
     */
    void CreateAccountDialog::_emitOk()
    {
        emit requested(_getAccount());
        close();
    }

    /**
     * @brief Create an account draft from the user input in the form, this will
     * take the values from the form fields and create a draft that can be used
     * by the controller to create a new account, this allows the dialog to
     * convert the raw user input into a structured format that can be easily
     * consumed by the rest of the application.
     *
     * @return drafts::AccountDraft The account draft created from the user
     * input
     */
    drafts::AccountDraft CreateAccountDialog::_getAccount()
    {
        return drafts::AccountDraft{
            .name = _nameLineEdit->text().toStdString(),
            .kind = AccountKindMeta::from_string(
                        _accountType->currentText().toStdString()
            )
                        .value(),
            .currency =
                CurrencyMeta::from_string(_currency->currentText().toStdString()
                )
                    .value()
        };
    }

}   // namespace ui