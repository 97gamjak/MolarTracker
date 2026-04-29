#ifndef __APP__INCLUDE__APP__REPOS__INSTRUMENT_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS__INSTRUMENT_REPO_HPP__

#include "app/repos/base_repo.hpp"
#include "app/repos_api/i_instrument_repo.hpp"

namespace app
{
    /**
     * @brief Repository for managing instruments in the database
     */
    class InstrumentRepo : public BaseRepo, public IInstrumentRepo
    {
       public:
        std::vector<std::string> getTickers() override;

        void addStock(const finance::Stock& stock) override;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS__INSTRUMENT_REPO_HPP__