#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__

#include "finance/instrument/option.hpp"

namespace drafts
{
    class CreateOptionTransactionDraft;
}   // namespace drafts

namespace mapper
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
}   // namespace mapper

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__OPTION_MAPPER_HPP__