#include "finance/transaction/option_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/instrument/option.hpp"
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
        std::optional<TransactionId> rolledOption,
        std::optional<std::string>   comment
    )
        : OptionTransactionTemporary(
              id,
              timestamp,
              status,
              instrumentId,
              securityAccount,
              cashAccount,
              externalAccount,
              quantity,
              amount,
              fees,
              positionId,
              action,
              buySell,
              rolledOption,
              std::move(comment)
          ),
          _underlyingInstrumentId(underlyingInstrumentId),
          _strikePrice(strikePrice),
          _contractSize(contractSize),
          _optionType(optionType)
    {
    }

    OptionTransaction::OptionTransaction(
        const OptionTransactionTemporary& tempTx,
        const finance::Option&            option
    )
        : OptionTransaction(
              tempTx.getId(),
              tempTx.getTimestamp(),
              tempTx.getStatus(),
              tempTx.getInstrumentId(),
              option.getUnderlying().getInstrumentId(),
              tempTx.getSecurityAccountId(),
              tempTx.getCashAccountId(),
              tempTx.getExternalAccountId(),
              tempTx.getQuantity(),
              option.getStrikePrice(),
              tempTx.getAmount(),
              tempTx.getFees(),
              option.getContractSize(),
              tempTx.getPositionId(),
              tempTx.getAction(),
              tempTx.getBuySell(),
              option.getOptionType(),
              tempTx.getRolledOption(),
              tempTx.getComment()
          )
    {
    }

    /**
     * @brief Get the base instrument ID for the option transaction, which is
     * the underlying instrument ID.
     *
     * @return InstrumentId The underlying instrument ID associated with the
     * option transaction.
     */
    InstrumentId OptionTransactionTemporary::getBaseInstrumentId() const
    {
        return getInstrumentId();
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
    TransactionEntries OptionTransactionTemporary::getEntries(
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
    OptionData OptionTransactionTemporary::getOptionData() const
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
                getBaseInstrumentId(),
                getQuantity(),
                getAmount(),
                getPositionId()
            }
        );
        return optionData;
    }

    std::int64_t OptionTransaction::getContractSize() const
    {
        return _contractSize;
    }

    std::optional<TransactionId> OptionTransactionTemporary::getRolledOption(
    ) const
    {
        return _rolledOption;
    }

    Cash OptionTransaction::getStrikePrice() const { return _strikePrice; }

    Cash OptionTransactionTemporary::getAmount() const { return _amount; }

    Currency OptionTransactionTemporary::getCurrency() const
    {
        return _amount.getCurrency();
    }

    OptionBuySell OptionTransactionTemporary::getBuySell() const
    {
        return _buySell;
    }

    OptionType OptionTransaction::getOptionType() const { return _optionType; }

    TransactionOptionAction OptionTransactionTemporary::getAction() const
    {
        return _action;
    }

    OptionTransactionTemporary::OptionTransactionTemporary(
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

}   // namespace finance