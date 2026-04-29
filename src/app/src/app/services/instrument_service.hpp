#ifndef __APP__SRC__APP__SERVICES__INSTRUMENT_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__INSTRUMENT_SERVICE_HPP__

#include <memory>

#include "app/services_api/i_instrument_service.hpp"

namespace finance
{
    class Stock;   // forward declaration
}   // namespace finance

namespace app
{
    class IInstrumentRepo;   // forward declaration

    /**
     * @brief Implementation of Instrument service
     *
     */
    class InstrumentService : public IInstrumentService
    {
       private:
        /// reference to the instrument repository
        std::shared_ptr<IInstrumentRepo> _instrumentRepo;

       public:
        explicit InstrumentService(
            const std::shared_ptr<IInstrumentRepo>& instrumentRepo
        );

        [[nodiscard]] std::vector<std::string> getTickers() override;

        void addStock(const finance::Stock& stock) override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__SERVICES__INSTRUMENT_SERVICE_HPP__