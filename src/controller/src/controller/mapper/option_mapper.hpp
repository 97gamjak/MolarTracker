#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__

#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/instrument/option.hpp"

namespace controller
{
    /**
     * @brief Mapper for converting option transaction drafts to option
     * instruments.
     *
     */
    class OptionMapper
    {
       public:
        [[nodiscard]]
        static finance::Option toOption(
            const drafts::CreateOptionTransactionDraft& draft,
            const finance::Stock&                       underlying
        );
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__