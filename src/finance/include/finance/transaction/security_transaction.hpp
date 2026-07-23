#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__SECURITY_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__SECURITY_TRANSACTION_HPP__

#include "common/cash.hpp"
#include "common/finance.hpp"
#include "common/quantity.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "finance/transaction/i_security_transaction.hpp"
#include "finance/transaction/transaction.hpp"

namespace finance
{
    /**
     * @brief Represents a transaction involving securities, which is a type of
     * transaction that involves the buying or selling of securities such as
     * stocks, bonds, or options. The SecurityTransaction class extends the
     * Transaction class and includes additional properties specific to security
     * transactions, such as the instrument being traded, the security account,
     * the quantity, and the position ID, and provides methods to access these
     * properties and perform operations related to the security transaction.
     *
     */
    class SecurityTransaction : public Transaction, ISecurityTransaction
    {
        /// The instrument ID of the option being traded in the transaction
        InstrumentId _instrumentId;

        /// The security account associated with the option transaction
        AccountId _securityAccount;

        /// The quantity of the option being traded in the transaction
        Quantity _quantity;

        /// The position ID associated with the option transaction
        PositionId _positionId;

       public:
        SecurityTransaction(
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
            std::optional<std::string> comment = std::nullopt
        );
        ~SecurityTransaction() override = default;

        [[nodiscard]] InstrumentId    getInstrumentId() const;
        [[nodiscard]] AccountId       getSecurityAccountId() const;
        [[nodiscard]] const Quantity& getQuantity() const;
        [[nodiscard]] PositionId      getPositionId() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__SECURITY_TRANSACTION_HPP__