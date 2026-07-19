#include "store/position_store.hpp"

#include <memory>

#include "exceptions/not_yet_implemented.hpp"
#include "finance/account/accounts.hpp"
#include "finance/positions.hpp"
#include "logging/log_macros.hpp"
#include "store/base/base_store.hpp"

REGISTER_LOG_CATEGORY("Store.PositionStore");

namespace store
{

    /**
     * @brief Session data for PositionStore
     *
     */
    struct PositionStore::Session
    {
        /// the account session
        const finance::Accounts& accountSession;

        /**
         * @brief Construct a new Session object
         *
         * @param accountSession_
         */
        explicit Session(const finance::Accounts& accountSession_)
            : accountSession(accountSession_)
        {
        }

        ~Session() = default;

        // delete copy and move
        Session(const Session&)            = delete;
        Session(Session&&)                 = delete;
        Session& operator=(const Session&) = delete;
        Session& operator=(Session&&)      = delete;
    };

    /**
     * @brief Construct a new Position Store:: Position Store object
     *
     * @param positionService
     * @param accountSession
     */
    PositionStore::PositionStore(
        std::shared_ptr<service::IPositionService> positionService,
        const finance::Accounts&                   accountSession
    )
        : _positionService(std::move(positionService)),
          _session(std::make_unique<Session>(accountSession))
    {
        const auto accountIds = _session->accountSession.getIds();
        if (!accountIds.empty())
        {
            const auto openPositions =
                _positionService->getAllOpenPositions(accountIds);

            _addCleanEntries(openPositions);
        }
    }

    PositionStore::~PositionStore() = default;

    /**
     * @brief Create a new position
     *
     * @param position
     *
     * @return PositionId
     */
    PositionId PositionStore::createPosition(const finance::Position& position)
    {
        return _addEntry(position);
    }

    /**
     * @brief Get all positions
     *
     * @return finance::Positions
     */
    finance::Positions PositionStore::getAllPositions() const
    {
        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        finance::Positions positions;
        for (const auto& position : _getValues(options))
            positions.addUnchecked(position);

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto dbPositions = _positionService->getAllPositions(accountIds);

        for (const auto& position : dbPositions)
            if (!ids.contains(position.getId()))
                positions.addUnchecked(position);

        return positions;
    }

    /**
     * @brief Get all open positions
     *
     * @return finance::Positions
     */
    finance::Positions PositionStore::getOpenPositions() const
    {
        LOG_ENTRY;

        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        auto options = Options{
            .filter   = finance::IsPositionOpen(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        finance::Positions positions;

        for (const auto& position : _getValues(options))
            positions.addUnchecked(position);

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto openPositions =
            _positionService->getAllOpenPositions(accountIds);

        for (const auto& position : openPositions)
            if (!ids.contains(position.getId()))
                positions.addUnchecked(position);

        LOG_DEBUG(
            std::format("Open positions retrieved: {}", positions.size())
        );

        return positions;
    }

    /**
     * @brief Commit all changes to the position store
     *
     */
    void PositionStore::commit()
    {
        _logCache(LOG_CATEGORY, LogLevel::Trace);

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto id =
                        _positionService->createPosition(entry.value);
                    auto newEntry = entry;
                    newEntry.value.setId(id);

                    _commitEntry(entry.value.getId(), newEntry);
                    break;
                }
                case StoreState::Clean:
                    break;
                case StoreState::Deleted:
                case StoreState::Modified:
                {
                    throw NotYetImplementedException(
                        "Position modification is not yet implemented"
                    );
                }
            }
        }

        _notifyOnCommit();
    }

    /**
     * @brief Get the ID remapping for positions
     *
     * @return const IdIdMap<PositionId>&
     */
    const IdIdMap<PositionId>& PositionStore::getIdRemap() const
    {
        return _getIdRemap();
    }

    /**
     * @brief Discard all cached positions and reload from the database.
     */
    void PositionStore::reload()
    {
        LOG_ENTRY;

        _logCache(LOG_CATEGORY, LogLevel::Debug);

        _clearEntries();
        const auto accountIds = _session->accountSession.getIds();
        if (!accountIds.empty())
        {
            const auto positions =
                _positionService->getAllOpenPositions(accountIds);
            _addCleanEntries(positions);
        }
    }

    /**
     * @brief Subscribe to position closed events, this allows subscribers to be
     * notified when a position is closed, which can be useful for updating
     *
     * @param func
     * @param user
     * @return Connection
     */
    Connection PositionStore::subscribeToPositionClosed(
        PositionClosed::func func,
        void*                user
    )
    {
        return _positionEvents->template on<PositionClosed>(
            std::move(func),
            user
        );
    }

}   // namespace store
