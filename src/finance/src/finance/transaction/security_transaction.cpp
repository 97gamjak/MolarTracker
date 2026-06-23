#include "finance/transaction/security_transaction.hpp"

namespace finance
{
    /**
     * @brief Construct a new Security Transaction:: Security Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param instrumentId
     * @param securityAccount
     * @param cashAccount
     * @param externalAccount
     * @param quantity
     * @param fees
     * @param positionId
     * @param comment
     */
    SecurityTransaction::SecurityTransaction(
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        InstrumentId               instrumentId,
        AccountId                  securityAccount,
        AccountId                  cashAccount,
        AccountId                  externalAccount,
        Quantity                   quantity,
        Cash                       fees,
        PositionId                 positionId,
        std::optional<std::string> comment
    )
        : Transaction(
              id,
              timestamp,
              status,
              cashAccount,
              externalAccount,
              TransactionDataType::Stock,   // TODO:
              fees,
              std::move(comment)
          ),
          _instrumentId(instrumentId),
          _securityAccount(securityAccount),
          _quantity(quantity),
          _positionId(positionId)
    {
    }

    /**
     * @brief Get the instrument ID of the security transaction.
     *
     * @return InstrumentId
     */
    InstrumentId SecurityTransaction::getInstrumentId() const
    {
        return _instrumentId;
    }

    /**
     * @brief Get the security account ID associated with the security
     * transaction.
     *
     * @return AccountId
     */
    AccountId SecurityTransaction::getSecurityAccountId() const
    {
        return _securityAccount;
    }

    /**
     * @brief Get the quantity associated with the security transaction.
     *
     * @return const Quantity&
     */
    const Quantity& SecurityTransaction::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Get the position ID associated with the security transaction.
     *
     * @return PositionId
     */
    PositionId SecurityTransaction::getPositionId() const
    {
        return _positionId;
    }

    IdSet<AccountId> SecurityTransaction::getInvolvedAccounts() const
    {
        IdSet<AccountId> accounts;
        accounts.insert(getCashAccountId());
        accounts.insert(getExternalAccountId());
        accounts.insert(getSecurityAccountId());
        return accounts;
    }
}   // namespace finance