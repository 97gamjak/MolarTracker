#include "finance/instrument/option.hpp"

namespace finance
{
    Option::Option(
        OptionId     id,
        InstrumentId instrumentId,
        Stock        underlying,
        OptionType   optionType,
        Cash         strikePrice,
        Timestamp    expirationDate
    )
        : _id(id),
          _instrumentId(instrumentId),
          _underlying(std::move(underlying)),
          _optionType(optionType),
          _strikePrice(strikePrice),
          _expirationDate(expirationDate)
    {
    }

    OptionId Option::getId() const { return _id; }

    InstrumentId Option::getInstrumentId() const { return _instrumentId; }

    OptionType Option::getOptionType() const { return _optionType; }

    Cash Option::getStrikePrice() const { return _strikePrice; }

    Timestamp Option::getExpirationDate() const { return _expirationDate; }

    const Stock& Option::getUnderlying() const { return _underlying; }

}   // namespace finance