#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/instrument/stock.hpp"
#include "utils/timestamp.hpp"

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
        OptionId _id;

        InstrumentId _instrumentId;

        Stock _underlying;

        OptionType _optionType;

        Cash _strikePrice;

        Timestamp _expirationDate;

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

        [[nodiscard]] OptionId     getId() const;
        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] OptionType   getOptionType() const;
        [[nodiscard]] Cash         getStrikePrice() const;
        [[nodiscard]] Timestamp    getExpirationDate() const;
        [[nodiscard]] const Stock& getUnderlying() const;
        [[nodiscard]] std::int64_t getContractSize() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] bool hasUnderlying(InstrumentId underlyingId) const;

        void updateUnderlying(InstrumentId underlyingId);

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