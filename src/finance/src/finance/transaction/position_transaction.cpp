#include "finance/transaction/position_transaction.hpp"

#include "finance/transaction/pnl.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Finance.StockPositionTransaction");

namespace finance
{

    StockPositionTransaction::StockPositionTransaction(PositionId id)
        : _positionId(id)
    {
    }

    bool StockPositionTransaction::add(const StockTransaction& txs)
    {
        if (!empty())
        {
            if (_baseInstrument != txs.getBaseInstrumentId())
            {
                LOG_ERROR(
                    "Failed to add stock transaction to position because it "
                    "does not match the base instrument"
                );
                return false;
            }

            if (_securityAccount != txs.getSecurityAccountId())
            {
                LOG_ERROR(
                    "Failed to add stock transaction to position because it "
                    "does not match the security account"
                );
                return false;
            }
        }

        _baseInstrument  = txs.getBaseInstrumentId();
        _securityAccount = txs.getSecurityAccountId();
        _pnlReady        = false;
        StockTransactions::add(txs);
        return true;
    }

    InstrumentId StockPositionTransaction::getBaseInstrument() const
    {
        return _baseInstrument;
    }

    AccountId StockPositionTransaction::getSecurityAccount() const
    {
        return _securityAccount;
    }

    PnL* StockPositionTransaction::getPnL()
    {
        if (!_pnl)
        {
            _pnl = std::make_unique<PnLAvg>();
        }

        if (!_pnlReady)
        {
            _pnl->calculatePnL(*this);
            _pnlReady = true;
        }

        return _pnl.get();
    }

}   // namespace finance
