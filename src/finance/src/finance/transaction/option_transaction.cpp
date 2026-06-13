#include "finance/transaction/option_transaction.hpp"

#include "config/id_types.hpp"

namespace finance
{

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
        PositionId                   positionId,
        TransactionOptionAction      action,
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
          _underlyingInstrumentId(underlyingInstrumentId),
          _strikePrice(strikePrice),
          _amount(amount),
          _action(action),
          _rolledOption(rolledOption)
    {
    }

    InstrumentId OptionTransaction::getBaseInstrumentId() const
    {
        return getInstrumentId();
    }

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
                getBaseInstrumentId(),
                getQuantity(),
                _strikePrice,
                getPositionId()
            }
        );
        return optionData;
    }

}   // namespace finance