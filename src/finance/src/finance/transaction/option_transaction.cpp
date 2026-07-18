#include "finance/transaction/option_transaction.hpp"

#include "config/id_types.hpp"
#include "utils/finance.hpp"

namespace finance
{

    /**
     * @brief Construct a new Option Transaction:: Option Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param instrumentId
     * @param underlyingInstrumentId
     * @param securityAccount
     * @param cashAccount
     * @param externalAccount
     * @param quantity
     * @param strikePrice
     * @param amount
     * @param fees
     * @param contractSize
     * @param positionId
     * @param action
     * @param buySell
     * @param optionType
     * @param rolledOption
     * @param comment
     */
    OptionTransaction::OptionTransaction(
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
        std::optional<TransactionId> rolledOption,
        std::optional<std::string>   comment
    )
        : SecurityTransaction(
              id,
              timestamp,
              status,
              instrumentId,
              securityAccount,
              cashAccount,
              externalAccount,
              quantity,
              fees,
              positionId,
              std::move(comment)
          ),
          _amount(amount),
          _action(action),
          _buySell(buySell),
          _rolledOption(rolledOption)
    {
    }

    /**
     * @brief Get the transaction entries for the option transaction, which
     * includes the cash movement for the option amount and fees.
     *
     * @param externalAccount The external account ID to use for the cash entry
     * representing the cash movement for the option amount.
     * @return TransactionEntries The transaction entries associated with the
     * option transaction.
     */
    TransactionEntries OptionTransaction::getEntries(
        AccountId externalAccount
    ) const
    {
        TransactionEntries entries;

        entries.add(
            TransactionEntry{
                TransactionEntryId::invalid(),
                getCashAccountId(),
                _amount,
                TransactionEntryType::General
            }
        );
        entries.add(
            TransactionEntry{
                TransactionEntryId::invalid(),
                externalAccount,
                -_amount,
                TransactionEntryType::General
            }
        );
        entries.add(_getFeeEntry());
        entries.add(_getFeeEntry(externalAccount));
        return entries;
    }

    /**
     * @brief Get the option data for the option transaction, which includes the
     * quantity, strike price, action, and any rolled option information.
     *
     * @return OptionData The option data associated with the option
     * transaction.
     */
    OptionData OptionTransaction::getOptionData() const
    {
        auto optionData = OptionData{
            TransactionOptionId::invalid(),   // populated with commit
            getQuantity() > Quantity{0} ? OptionBuySell::Buy
                                        : OptionBuySell::Sell,
            _action,
            _rolledOption
        };

        optionData.addLeg(
            TradeLeg{
                getSecurityAccountId(),
                getInstrumentId(),
                getQuantity(),
                _amount,
                getPositionId()
            }
        );
        return optionData;
    }

}   // namespace finance