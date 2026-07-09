#include "finance/transaction/position_transaction.hpp"

#include "finance/transaction/pnl.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Finance.StockPositionTransaction");

namespace finance
{

    /**
     * @brief Construct a new Stock Position Transaction:: Stock Position
     * Transaction object
     *
     * @param id
     */
    StockPositionTransaction::StockPositionTransaction(PositionId id)
        : _positionId(id)
    {
    }

    /**
     * @brief Add a stock transaction to the position, this will check that the
     * transaction matches the base instrument and security account of the
     * position, and if so it will add the transaction to the list of stock
     * transactions for the position and mark the PnL as not ready for
     * recalculation.
     *
     * @param txs The stock transaction to add to the position.
     * @return true if the transaction was added successfully, false otherwise.
     */
    bool StockPositionTransaction::addPosition(const StockTransaction& txs)
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
        StockTransactionsView::add(txs);
        return true;
    }

    /**
     * @brief get the base instrument associated with the position
     *
     * @return InstrumentId
     */
    InstrumentId StockPositionTransaction::getBaseInstrument() const
    {
        return _baseInstrument;
    }

    /**
     * @brief Get the security account associated with the position, this will
     * be used for display purposes and to ensure that transactions added to the
     * position match the correct account.
     *
     * @return AccountId The security account ID associated with the position.
     */
    AccountId StockPositionTransaction::getSecurityAccount() const
    {
        return _securityAccount;
    }

    /**
     * @brief Get the PnL information for the position, this will calculate the
     * PnL based on the transactions in the position if it is not already
     * calculated, and return a shared pointer to the PnL object for display and
     * further calculations.
     *
     * @return const std::shared_ptr<PnL>& A shared pointer to the PnL object
     * containing the profit and loss information for the position.
     */
    const std::shared_ptr<PnL>& StockPositionTransaction::getPnL()
    {
        if (!_pnl)
        {
            _pnl = std::make_shared<PnLAvg>();
        }

        if (!_pnlReady)
        {
            _pnl->calculatePnL(*this);
            _pnlReady = true;
        }

        return _pnl;
    }

}   // namespace finance
