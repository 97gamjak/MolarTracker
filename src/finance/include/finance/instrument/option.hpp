#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "finance/cash.hpp"
#include "finance/instrument/stock.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    class Option
    {
       private:
        OptionId _id;

        InstrumentId _instrumentId;

        Stock _underlying;

        OptionType _optionType;

        Cash _strikePrice;

        Timestamp _expirationDate;

       public:
        Option(
            OptionId     id,
            InstrumentId instrumentId,
            Stock        underlying,
            OptionType   optionType,
            Cash         strikePrice,
            Timestamp    expirationDate
        );

        [[nodiscard]] OptionId     getId() const;
        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] OptionType   getOptionType() const;
        [[nodiscard]] Cash         getStrikePrice() const;
        [[nodiscard]] Timestamp    getExpirationDate() const;
        [[nodiscard]] const Stock& getUnderlying() const;

        [[nodiscard]] std::string getName() const;

        void setId(OptionId id);
        void setInstrumentId(InstrumentId instrumentId);

        [[nodiscard]] std::string toString() const;
    };

    struct [[nodiscard]] OptionInsertionResult
    {
        /// The ID of the option that was inserted
        OptionId optionId;
        /// The ID of the instrument that was generated for the option
        InstrumentId instrumentId;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTION_HPP__