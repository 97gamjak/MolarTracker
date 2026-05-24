#include "app/service_container.hpp"

#include "repo/repo_container.hpp"
#include "services/account_service.hpp"
#include "services/instrument_service.hpp"
#include "services/position_service.hpp"
#include "services/profile_service.hpp"
#include "services/transaction_service.hpp"

namespace app
{

    /**
     * @brief Construct a new Service Container object
     *
     * @param repos
     */
    ServiceContainer::ServiceContainer()
        : _repoContainer{std::make_unique<repo::RepoContainer>()},
          _profileService{
              std::make_shared<ProfileService>(_repoContainer->getProfileRepo())
          },
          _accountService{
              std::make_shared<AccountService>(_repoContainer->getAccountRepo())
          },
          _transactionService{std::make_shared<TransactionService>(
              _repoContainer->getTransactionRepo()
          )},
          _instrumentService{std::make_shared<InstrumentService>(
              _repoContainer->getInstrumentRepo()
          )},
          _positionService{std::make_shared<PositionService>(
              _repoContainer->getPositionRepo()
          )}

    {
    }

    ServiceContainer::~ServiceContainer() = default;

    /**
     * @brief Get the Profile Service
     *
     * @return std::shared_ptr<IProfileService>
     */
    std::shared_ptr<IProfileService> ServiceContainer::getProfileService()
    {
        return _profileService;
    }

    /**
     * @brief Get the Profile Service (const version)
     *
     * @return std::shared_ptr<const IProfileService>
     */
    std::shared_ptr<const IProfileService> ServiceContainer::getProfileService(
    ) const
    {
        return _profileService;
    }

    /**
     * @brief Get the Account Service
     *
     * @return std::shared_ptr<IAccountService>
     */
    std::shared_ptr<IAccountService> ServiceContainer::getAccountService()
    {
        return _accountService;
    }

    /**
     * @brief Get the Account Service (const version)
     *
     * @return std::shared_ptr<const IAccountService>
     */
    std::shared_ptr<const IAccountService> ServiceContainer::getAccountService(
    ) const
    {
        return _accountService;
    }

    /**
     * @brief Get the Transaction Service
     *
     * @return std::shared_ptr<ITransactionService>
     */
    std::shared_ptr<ITransactionService> ServiceContainer::
        getTransactionService()
    {
        return _transactionService;
    }

    /**
     * @brief Get the Transaction Service (const version)
     *
     * @return std::shared_ptr<const ITransactionService>
     */
    std::shared_ptr<const ITransactionService> ServiceContainer::
        getTransactionService() const
    {
        return _transactionService;
    }

    /**
     * @brief Get the Instrument Service
     *
     * @return std::shared_ptr<IInstrumentService>
     */
    std::shared_ptr<IInstrumentService> ServiceContainer::getInstrumentService()
    {
        return _instrumentService;
    }

    /**
     * @brief Get the Instrument Service (const version)
     *
     * @return std::shared_ptr<const IInstrumentService>
     */
    std::shared_ptr<const IInstrumentService> ServiceContainer::
        getInstrumentService() const
    {
        return _instrumentService;
    }

    /**
     * @brief Get the Position Service
     *
     * @return std::shared_ptr<IPositionService>
     */
    std::shared_ptr<IPositionService> ServiceContainer::getPositionService()
    {
        return _positionService;
    }

    /**
     * @brief Get the Position Service (const version)
     *
     * @return std::shared_ptr<const IPositionService>
     */
    std::shared_ptr<const IPositionService> ServiceContainer::
        getPositionService() const
    {
        return _positionService;
    }

}   // namespace app