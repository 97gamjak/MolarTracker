#include "finance/instrument/option.hpp"

#include <format>
#include <string>

namespace finance
{
    /**
     * @brief Construct a new Option:: Option object
     *
     * @param id
     * @param instrumentId
     * @param underlying
     * @param optionType
     * @param strikePrice
     * @param expirationDate
     * @param contractSize
     */
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

    /**
     * @brief Get the ID of the option.
     *
     * @return OptionId
     */
    OptionId Option::getId() const { return _id; }

    /**
     * @brief Get the instrument ID associated with the option.
     *
     * @return InstrumentId
     */
    InstrumentId Option::getInstrumentId() const { return _instrumentId; }

    /**
     * @brief Get the option type (call or put) of the option.
     *
     * @return OptionType
     */
    OptionType Option::getOptionType() const { return _optionType; }

    /**
     * @brief Get the strike price of the option.
     *
     * @return Cash
     */
    Cash Option::getStrikePrice() const { return _strikePrice; }

    /**
     * @brief Get the expiration date of the option.
     *
     * @return Timestamp
     */
    Timestamp Option::getExpirationDate() const { return _expirationDate; }

    /**
     * @brief Get the underlying stock of the option.
     *
     * @return const Stock&
     */
    const Stock& Option::getUnderlying() const { return _underlying; }

    /**
     * @brief Get the contract size of the option.
     *
     * @return std::int64_t
     */
    std::int64_t Option::getContractSize() const { return _contractSize; }

    /**
     * @brief Get the name of the option, which is typically a combination of
     * the underlying ticker, expiration date, option type, and strike price,
     * this provides a human-readable identifier for the option that can be used
     * for display purposes or for referencing the option in various contexts.
     *
     * @return std::string
     */
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

    /**
     * @brief Check if the option has a specific underlying stock based on its
     * instrument ID, this allows for verifying if the option is associated with
     * a particular underlying stock, which can be useful for filtering or
     * categorizing options based on their underlying assets.
     *
     * @param underlyingId The instrument ID of the underlying stock to check.
     * @return bool True if the option has the specified underlying stock, false
     * otherwise.
     */
    bool Option::hasUnderlying(InstrumentId underlyingId) const
    {
        return _underlying.getInstrumentId() == underlyingId;
    }

    /**
     * @brief Update the underlying stock of the option by setting a new
     * instrument ID, this allows for changing the association of the option to
     * a different underlying stock, which can be useful in cases where the
     * underlying stock has been updated or if there was an error in the initial
     * association that needs to be corrected.
     *
     * @param underlyingId The new instrument ID to associate with the option's
     * underlying stock.
     */
    void Option::updateUnderlying(InstrumentId underlyingId)
    {
        _underlying.setInstrumentId(underlyingId);
    }

    /**
     * @brief Set the ID of the option, this allows for assigning a unique
     * identifier to the option, which can be useful for tracking and
     * referencing the option in various parts of the application, such as when
     * storing it in a database or when performing operations that require
     * identifying the option by its ID.
     *
     * @param id The new ID to set for the option.
     */
    void Option::setId(OptionId id) { _id = id; }

    /**
     * @brief Set the instrument ID associated with the option, this allows for
     * changing the association of the option to a different instrument, which
     * can be useful in cases where the instrument has been updated or if there
     * was an error in the initial association that needs to be corrected.
     *
     * @param instrumentId The new instrument ID to associate with the option.
     */
    void Option::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    /**
     * @brief Get a string representation of the option, this provides a human-
     * readable description of the option that includes its key attributes such
     * as ID, instrument ID, underlying stock, option type, strike price, and
     * expiration date, which can be useful for logging, debugging, or
     * displaying the option's information in the user interface.
     *
     * @return std::string A string representation of the option.
     */
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