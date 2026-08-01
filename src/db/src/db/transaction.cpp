#include "db/transaction.hpp"

#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("DB.Transaction");

namespace db
{

    /**
     * @brief Construct a new Transaction:: Transaction object
     *
     * @param db
     * @param immediate
     */
    Transaction::Transaction(Database& db, bool immediate) : _db(&db)
    {
        if (!_db->isTransactionStarted())
        {
            const auto result = _db->begin(immediate);

            if (!result)
                throw SqliteError(result.error().toString());

            _isActive = true;
        }
        else
        {
            _isActive = false;
        }
    }

    /**
     * @brief Construct a new Transaction:: Transaction object with immediate
     * mode by default
     *
     * @param db
     */
    Transaction::Transaction(Database& db) : Transaction(db, true) {}

    /**
     * @brief Destroy the Transaction:: Transaction object
     *
     */
    Transaction::~Transaction()
    {
        if (_isActive && _db != nullptr)
        {
            try
            {
                const auto result = rollback();
                if (!result)
                {
                    LOG_ERROR(
                        std::format(
                            "Failed to rollback transaction in destructor: {}",
                            result.error().toString()
                        )
                    );
                }
            }
            catch (const std::exception& e)
            {
                LOG_ERROR(
                    std::format(
                        "Failed to rollback transaction in destructor: {}",
                        e.what()
                    )
                );
            }
        }
    }

    /**
     * @brief Move constructor
     *
     * @param other
     */
    Transaction::Transaction(Transaction&& other) noexcept
        : _db(other._db), _isActive(other._isActive)
    {
        _moveFrom(std::move(other));
    }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Transaction&
     */
    Transaction& Transaction::operator=(Transaction&& other) noexcept
    {
        if (this != &other)
            _moveFrom(std::move(other));

        return *this;
    }

    /**
     * @brief check if the transaction is active
     *
     * @return true
     * @return false
     */
    bool Transaction::isActive() const { return _isActive; }

    /**
     * @brief commit the transaction
     *
     * @return DatabaseResult<void>
     */
    DatabaseResult<void> Transaction::commit()
    {
        if (!_isActive || _db == nullptr)
            return {};

        const auto result = _db->commit();
        if (!result)
            return result.error();

        _isActive = false;
        return {};
    }

    /**
     * @brief rollback the transaction
     *
     * @return DatabaseResult<void>
     */
    DatabaseResult<void> Transaction::rollback()
    {
        if (!_isActive || _db == nullptr)
            return {};

        const auto result = _db->rollback();
        if (!result)
            return result.error();

        _isActive = false;
        return {};
    }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    /**
     * @brief Move helper method
     *
     * @param other
     */
    // NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
    void Transaction::_moveFrom(Transaction&& other)
    {
        _db       = other._db;
        _isActive = other._isActive;

        other._db       = nullptr;
        other._isActive = false;
    }

}   // namespace db
