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
     * @param securityAccount
     * @param cashAccount
     * @param externalAccount
     * @param quantity
     * @param amount
     * @param fees
     * @param positionId
     * @param action
     * @param buySell
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
              TransactionDataType::Option,
              securityAccount,
              cashAccount,
              externalAccount,
              quantity,
              fees,
              positionId,
              std::move(comment)
          ),
          _premium(amount),
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
                _premium,
                TransactionEntryType::General
            }
        );
        entries.add(
            TransactionEntry{
                TransactionEntryId::invalid(),
                externalAccount,
                -_premium,
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
            _buySell,
            _action,
            _rolledOption
        };

        optionData.addLeg(
            TradeLeg{
                getSecurityAccountId(),
                getInstrumentId(),
                getQuantity(),
                _premium,
                getPositionId()
            }
        );
        return optionData;
    }

    /**
     * @brief Get the buy/sell direction of the option transaction, which
     * indicates whether the option is being bought or sold.
     *
     * @return const OptionBuySell& The buy/sell direction of the option
     * transaction.
     */
    const OptionBuySell& OptionTransaction::getBuySell() const
    {
        return _buySell;
    }

    /**
     * @brief Get the action being performed in the option transaction, which
     * indicates whether the option is being opened, closed, or rolled.
     *
     * @return const TransactionOptionAction& The action of the option
     * transaction.
     */
    const TransactionOptionAction& OptionTransaction::getAction() const
    {
        return _action;
    }

    /**
     * @brief Get the premium amount of the option transaction, which represents
     * the cash flow associated with the option trade, either paid or received.
     *
     * @return const Cash& The premium amount of the option transaction.
     */
    const Cash& OptionTransaction::getPremium() const { return _premium; }

}   // namespace finance