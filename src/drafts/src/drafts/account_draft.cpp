#include "drafts/account_draft.hpp"

#include "config/id_types.hpp"

namespace drafts
{

    /**
     * @brief Construct a new Account Draft:: Account Draft object
     *
     * @param _name
     * @param _kind
     * @param _currency
     */
    AccountDraft::AccountDraft(
        std::string _name,
        AccountKind _kind,
        Currency    _currency
    )
        : name(std::move(_name)), kind(_kind), currency(_currency)
    {
    }

    /**
     * @brief Construct a new Account Draft:: Account Draft object
     *
     * @param _id
     * @param _name
     * @param _kind
     * @param _currency
     * @param _status
     */
    AccountDraft::AccountDraft(
        AccountId                    _id,
        std::string                  _name,
        AccountKind                  _kind,
        Currency                     _currency,
        std::optional<AccountStatus> _status
    )
        : id(_id),
          name(std::move(_name)),
          kind(_kind),
          currency(_currency),
          status(_status)
    {
    }

}   // namespace drafts