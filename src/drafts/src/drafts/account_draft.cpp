#include "drafts/account_draft.hpp"

#include "config/id_types.hpp"

namespace drafts
{

    /**
     * @brief Construct a new Account Draft:: Account Draft object
     *
     * @param id
     * @param name
     * @param kind
     * @param currency
     * @param status
     */
    AccountDraft::AccountDraft(
        AccountId                    id,
        std::optional<AccountStatus> status,
        std::string                  name,
        Currency                     currency,
        AccountKind                  kind
    )
        : _id(id),
          _status(status),
          _name(std::move(name)),
          _currency(currency),
          _kind(kind)
    {
    }

    /**
     * @brief get the ID of the account draft
     * @return AccountId
     */
    AccountId AccountDraft::getId() const { return _id; }

    /**
     * @brief get the status of the account draft
     *
     * @return std::optional<AccountStatus>
     */
    std::optional<AccountStatus> AccountDraft::getStatus() const
    {
        return _status;
    }

    /**
     * @brief get the name of the account draft
     *
     * @return const std::string&
     */
    const std::string& AccountDraft::getName() const { return _name; }

    /**
     * @brief get the kind of the account draft
     *
     * @return AccountKind
     */
    AccountKind AccountDraft::getKind() const { return _kind; }

    /**
     * @brief get the currency of the account draft
     *
     * @return Currency
     */
    Currency AccountDraft::getCurrency() const { return _currency; }

}   // namespace drafts