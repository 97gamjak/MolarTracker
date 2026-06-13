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
              fees,
              std::move(comment)
          ),
          _instrumentId(instrumentId),
          _securityAccount(securityAccount),
          _quantity(quantity),
          _positionId(positionId)
    {
    }

    [[nodiscard]] InstrumentId SecurityTransaction::getInstrumentId() const
    {
        return _instrumentId;
    }

    [[nodiscard]] AccountId SecurityTransaction::getSecurityAccountId() const
    {
        return _securityAccount;
    }

    [[nodiscard]] const Quantity& SecurityTransaction::getQuantity() const
    {
        return _quantity;
    }

    [[nodiscard]] PositionId SecurityTransaction::getPositionId() const
    {
        return _positionId;
    }
}   // namespace finance