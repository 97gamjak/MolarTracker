#include "finance/transaction/position_transaction.hpp"

#include <utility>

#include "config/id_types.hpp"
#include "finance/positions.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/pnl.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Finance.StockPositionTransaction");

namespace finance
{
    PositionTransaction::PositionTransaction(
        const Position&     position,
        InstrumentId        baseInstrument,
        AccountId           securityAccount,
        InstrumentType      instrumentType,
        const Transactions& transactions
    )
        : _position(position),
          _baseInstrument(baseInstrument),
          _securityAccount(securityAccount),
          _instrumentType(instrumentType),
          _transactions(transactions)
    {
    }

    const std::shared_ptr<PnL>& PositionTransaction::getPnL()
    {
        if (!_pnl)
        {
            _pnl = std::make_shared<PnLAvg>();
        }

        if (!_pnlReady)
        {
            switch (getInstrumentType())
            {
                case InstrumentType::Stock:
                    _pnl->calculatePnL(_transactions.stocks());
                    break;
                case InstrumentType::Option:
                {
                    // TODO: combine pnls!
                    const auto optionsResult = _transactions.options();
                    if (!optionsResult)
                    {
                        LOG_ERROR(
                            std::format(
                                "Failed to calculate PnL for position {}: {}",
                                getId().toString(),
                                optionsResult.error().message
                            )
                        );
                        break;
                    }
                    _pnl->calculatePnL(optionsResult.value());
                    break;
                }
            }
            _pnlReady = true;
        }

        return _pnl;
    }

    PositionTransaction PositionTransaction::fromTransactions(
        const Position&     position,
        InstrumentType      instrumentType,
        const Transactions& transactions
    )
    {
        switch (instrumentType)
        {
            case InstrumentType::Stock:
            {
                PositionTransaction result(
                    position,
                    transactions.stocks()[0].getInstrumentId(),
                    transactions.stocks()[0].getSecurityAccountId(),
                    instrumentType,
                    transactions
                );

                return result;
            }
            case InstrumentType::Option:
            {
                PositionTransaction result(
                    position,
                    transactions.options().value()[0].getInstrumentId(),
                    transactions.options().value()[0].getSecurityAccountId(),
                    instrumentType,
                    transactions
                );
                return result;
            }
        }

        std::unreachable();
    }

    PositionTransactions PositionTransactions::fromTransactions(
        const Transactions& transactions,
        const Positions&    positions
    )
    {
        const IdMap<PositionId, Transactions> groupedTransactions =
            transactions.groupByPosition();

        PositionTransactions positionTransactions;

        for (const auto& [positionId, txs] : groupedTransactions)
        {
            if (!positions.contains(positionId))
                continue;

            if (txs.containsOptions())
            {
                positionTransactions._optionPositions.push_back(
                    PositionTransaction::fromTransactions(
                        positions.at(positionId),
                        InstrumentType::Option,
                        txs
                    )
                );
            }
            else
            {
                positionTransactions._stockPositions.push_back(
                    PositionTransaction::fromTransactions(
                        positions.at(positionId),
                        InstrumentType::Stock,
                        txs
                    )
                );
            }
        }

        return positionTransactions;
    }

    std::vector<PositionTransaction> PositionTransactions::getStockPositions(
    ) const
    {
        return _stockPositions;
    }

    std::vector<PositionTransaction> PositionTransactions::getOptionPositions(
    ) const
    {
        return _optionPositions;
    }

    std::vector<PositionTransaction> PositionTransactions::getAllPositions(
    ) const
    {
        std::vector<PositionTransaction> allPositions;
        allPositions.reserve(_stockPositions.size() + _optionPositions.size());

        allPositions.insert(
            allPositions.end(),
            _stockPositions.begin(),
            _stockPositions.end()
        );
        allPositions.insert(
            allPositions.end(),
            _optionPositions.begin(),
            _optionPositions.end()
        );

        return allPositions;
    }

    InstrumentId PositionTransaction::getBaseInstrument() const
    {
        return _baseInstrument;
    }

    AccountId PositionTransaction::getSecurityAccount() const
    {
        return _securityAccount;
    }

    InstrumentType PositionTransaction::getInstrumentType() const
    {
        return _instrumentType;
    }

    PositionId PositionTransaction::getId() const { return _position.getId(); }

    const Position& PositionTransaction::getPosition() const
    {
        return _position;
    }

}   // namespace finance
