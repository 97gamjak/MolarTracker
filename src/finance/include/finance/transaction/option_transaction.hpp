#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__

#include "config/id_types.hpp"
#include "finance/transaction/option_data.hpp"
#include "finance/transaction/security_transaction.hpp"
#include "utils/cash.hpp"
#include "utils/finance.hpp"
#include "utils/quantity.hpp"

namespace finance
{
    /**
     * @brief Represents a transaction involving options, which is a type of
     * security transaction that involves the buying or selling of options
     * contracts. The OptionTransaction class extends the SecurityTransaction
     * class and includes additional properties specific to option transactions,
     * such as the underlying instrument, strike price, amount, and the action
     * being performed (e.g., opening a new position, closing an existing
     * position, rolling an option, etc.), and provides methods to access these
     * properties and perform operations related to the option transaction.
     *
     */
    class OptionTransaction : public SecurityTransaction
    {
       private:
        /// The amount of the option being traded in the transaction
        Cash _amount;

        /// The action being performed in the option transaction (e.g., open,
        /// close, roll)
        TransactionOptionAction _action;

        OptionBuySell _buySell;

        /// The ID of the rolled option transaction, if this transaction is a
        /// roll of an existing option position, this allows the transaction to
        /// reference the original option transaction that is being rolled,
        /// which can be useful for tracking the history of the position and
        /// understanding the sequence of transactions that have occurred for
        /// the position, and provides a way to link the new option transaction
        /// to the original transaction that is being rolled, allowing for
        /// better traceability and analysis of the option trading activity.
        std::optional<TransactionId> _rolledOption;

       public:
        OptionTransaction(
            TransactionId                id,
            Timestamp                    timestamp,
            TransactionStatus            status,
            InstrumentId                 instrumentId,
            AccountId                    securityAccount,
            AccountId                    cashAccount,
            AccountId                    externalAccount,
            Quantity                     quantity,
            Cash                         amount,
            Cash                         fees,
            PositionId                   positionId,
            TransactionOptionAction      action,
            OptionBuySell                buySell,
            std::optional<TransactionId> rolledOption = std::nullopt,
            std::optional<std::string>   comment      = std::nullopt
        );

        [[nodiscard]]
        TransactionEntries getEntries(AccountId externalAccount) const override;

        [[nodiscard]]
        OptionData getOptionData() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__