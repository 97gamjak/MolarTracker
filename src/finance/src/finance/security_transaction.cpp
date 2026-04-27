#include "finance/security_transaction.hpp"

namespace finance
{
    StockTransaction::StockTransaction(
        std::string ticker,
        Currency    currency,
        micro_units quantity
    )
        : _ticker(std::move(ticker)), _currency(currency), _quantity(quantity)
    {
    }

    const std::string& StockTransaction::getTicker() const { return _ticker; }

    Currency StockTransaction::getCurrency() const { return _currency; }

    micro_units StockTransaction::getQuantity() const { return _quantity; }
}   // namespace finance
