#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__

#include <cstddef>

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
        /// The underlying instrument ID of the option being traded in the
        /// transaction
        InstrumentId _underlyingInstrumentId;

        /// The strike price of the option being traded in the transaction
        Cash _strikePrice;
        /// The amount of the option being traded in the transaction
        Cash _amount;

        std::int64_t _contractSize;

        /// The action being performed in the option transaction (e.g., open,
        /// close, roll)
        TransactionOptionAction _action;

        OptionBuySell _buySell;

        OptionType _optionType;

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
            InstrumentId                 underlyingInstrumentId,
            AccountId                    securityAccount,
            AccountId                    cashAccount,
            AccountId                    externalAccount,
            Quantity                     quantity,
            Cash                         strikePrice,
            Cash                         amount,
            Cash                         fees,
            std::int64_t                 contractSize,
            PositionId                   positionId,
            TransactionOptionAction      action,
            OptionBuySell                buySell,
            OptionType                   optionType,
            std::optional<TransactionId> rolledOption = std::nullopt,
            std::optional<std::string>   comment      = std::nullopt
        );

        [[nodiscard]] InstrumentId getBaseInstrumentId() const override;

        [[nodiscard]]
        TransactionEntries getEntries(AccountId externalAccount) const override;

        [[nodiscard]] OptionData                   getOptionData() const;
        [[nodiscard]] std::int64_t                 getContractSize() const;
        [[nodiscard]] Cash                         getStrikePrice() const;
        [[nodiscard]] Cash                         getAmount() const;
        [[nodiscard]] TransactionOptionAction      getAction() const;
        [[nodiscard]] OptionBuySell                getBuySell() const;
        [[nodiscard]] OptionType                   getOptionType() const;
        [[nodiscard]] std::optional<TransactionId> getRolledOption() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_TRANSACTION_HPP__