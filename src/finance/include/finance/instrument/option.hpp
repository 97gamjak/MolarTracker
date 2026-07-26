#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__

#include "common/cash.hpp"
#include "common/finance.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "finance/instrument/stock.hpp"

namespace finance
{
    /**
     * @brief Represents an option financial instrument, which is a derivative
     * that gives the holder the right, but not the obligation, to buy or sell
     * an underlying asset at a specified price (strike price) on or before a
     * specified date (expiration date). The Option class encapsulates the
     * properties of an option, such as its type (call or put), strike price,
     * expiration date, and the underlying stock, and provides methods to access
     * these properties and perform operations related to the option, such as
     * calculating its name or checking if it has a specific underlying stock.
     *
     */
    class Option
    {
       private:
        /// The unique identifier for the option
        OptionId _id;

        /// The unique identifier for the instrument associated with the option
        InstrumentId _instrumentId;

        /// The underlying stock associated with the option
        Stock _underlying;

        /// The option type (call or put) of the option
        OptionType _optionType;

        /// The strike price of the option
        Cash _strikePrice;

        /// The expiration date of the option
        Timestamp _expirationDate;

        /// The contract size of the option
        std::int64_t _contractSize;

       public:
        Option(
            OptionId     id,
            InstrumentId instrumentId,
            Stock        underlying,
            OptionType   optionType,
            Cash         strikePrice,
            Timestamp    expirationDate,
            std::int64_t contractSize
        );

        [[nodiscard]] OptionId            getId() const;
        [[nodiscard]] InstrumentId        getInstrumentId() const;
        [[nodiscard]] OptionType          getOptionType() const;
        [[nodiscard]] Cash                getStrikePrice() const;
        [[nodiscard]] Timestamp           getExpirationDate() const;
        [[nodiscard]] const std::int64_t& getContractSize() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] const Stock& getUnderlying() const;
        [[nodiscard]] bool hasUnderlying(InstrumentId underlyingId) const;
        void               updateUnderlying(InstrumentId underlyingId);

        void setId(OptionId id);
        void setInstrumentId(InstrumentId instrumentId);

        [[nodiscard]] std::string toString() const;
    };

    /**
     * @brief Struct representing the result of inserting an option into the
     * database, this is used to encapsulate the information about the newly
     * created option, including its unique identifier (optionId) and the
     * identifier of the associated instrument (instrumentId), which can be
     * useful for tracking and referencing the option in various parts of the
     * application after it has been created.
     *
     */
    struct [[nodiscard]] OptionInsertionResult
    {
        /// The ID of the option that was inserted
        OptionId optionId;
        /// The ID of the instrument that was generated for the option
        InstrumentId instrumentId;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__