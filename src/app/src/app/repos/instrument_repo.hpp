#ifndef __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__
#define __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__

#include "app/repos_api/i_instrument_repo.hpp"
#include "base_repo.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{
    /**
     * @brief Implementation of the instrument repository
     */
    class InstrumentRepo : public IInstrumentRepo, public BaseRepo
    {
       public:
        explicit InstrumentRepo(db::Database& db);

        std::optional<InstrumentId> getInstrumentId(
            const InstrumentRow& row
        ) override;

       private:
        std::expected<InstrumentId, orm::CrudError> _createInstrument(
            const InstrumentRow& row
        );

        std::expected<InstrumentId, orm::CrudError> _createInstrumentIfNotExists(
            const InstrumentRow& row
        );
    };
}   // namespace app

#endif   // __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__
