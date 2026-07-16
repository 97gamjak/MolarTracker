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

    /**
     * @brief Get the instrument ID of the security transaction.
     *
     * @return InstrumentId
     */
    [[nodiscard]] InstrumentId SecurityTransaction::getInstrumentId() const
    {
        return _instrumentId;
    }

    /**
     * @brief Get the security account ID associated with the security
     * transaction.
     *
     * @return AccountId
     */
    [[nodiscard]] AccountId SecurityTransaction::getSecurityAccountId() const
    {
        return _securityAccount;
    }

    /**
     * @brief Get the quantity associated with the security transaction.
     *
     * @return const Quantity&
     */
    [[nodiscard]] const Quantity& SecurityTransaction::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Get the position ID associated with the security transaction.
     *
     * @return PositionId
     */
    [[nodiscard]] PositionId SecurityTransaction::getPositionId() const
    {
        return _positionId;
    }
}   // namespace finance