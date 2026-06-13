#include "option_mapper.hpp"

namespace controller
{
    finance::Option OptionMapper::toOption(
        const drafts::CreateOptionTransactionDraft& draft,
        const finance::Stock&                       underlying
    )
    {
        return {
            OptionId::invalid(),
            InstrumentId::invalid(),
            underlying,
            draft.getOptionType(),
            draft.getStrikePrice(),
            draft.getExpiration(),
            draft.getContractSize()
        };
    }
}   // namespace controller