#include "finance/instrument/option.hpp"

#include <string>

namespace finance
{
    Option::Option(
        OptionId     id,
        InstrumentId instrumentId,
        Stock        underlying,
        OptionType   optionType,
        Cash         strikePrice,
        Timestamp    expirationDate,
        std::int64_t contractSize
    )
        : _id(id),
          _instrumentId(instrumentId),
          _underlying(std::move(underlying)),
          _optionType(optionType),
          _strikePrice(strikePrice),
          _expirationDate(expirationDate),
          _contractSize(contractSize)
    {
    }

    OptionId Option::getId() const { return _id; }

    InstrumentId Option::getInstrumentId() const { return _instrumentId; }

    OptionType Option::getOptionType() const { return _optionType; }

    Cash Option::getStrikePrice() const { return _strikePrice; }

    Timestamp Option::getExpirationDate() const { return _expirationDate; }

    const Stock& Option::getUnderlying() const { return _underlying; }

    std::int64_t Option::getContractSize() const { return _contractSize; }

    std::string Option::getName() const
    {
        return std::format(
            "{}{}{}{}",
            _underlying.getTicker(),
            _expirationDate.toDateString(),
            _optionType == OptionType::Call ? "C" : "P",
            _strikePrice.toString(2, false, false)
        );
    }

    bool Option::hasUnderlying(InstrumentId underlyingId) const
    {
        return _underlying.getInstrumentId() == underlyingId;
    }

    void Option::updateUnderlying(InstrumentId underlyingId)
    {
        _underlying.setInstrumentId(underlyingId);
    }

    void Option::setId(OptionId id) { _id = id; }

    void Option::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    std::string Option::toString() const
    {
        return std::format(
            "Option{{id: {}, instrumentId: {}, underlying: {}, optionType: "
            "{}, strikePrice: {}, expirationDate: {}}}",
            _id.toString(),
            _instrumentId.toString(),
            _underlying.toString(),
            OptionTypeMeta::toString(_optionType),
            _strikePrice.toString(2, false, false),
            _expirationDate.toDateString()
        );
    }

}   // namespace finance