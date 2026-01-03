#include "transaction.hpp"

#include "database.hpp"

namespace db
{

    /**
     * @brief Construct a new Transaction:: Transaction object
     *
     * @param db
     * @param immediate
     */
    Transaction::Transaction(Database& db, bool immediate)
        : _db(&db), _is_active(true)
    {
        if (immediate)
            _db->execute("BEGIN IMMEDIATE;");
        else
            _db->execute("BEGIN;");
    }

    /**
     * @brief Destroy the Transaction:: Transaction object
     *
     */
    Transaction::~Transaction() noexcept
    {
        if (_is_active && _db != nullptr)
            try
            {
                rollback();
            }
            catch (...)
            {
            }
    }

    /**
     * @brief Move constructor
     *
     * @param other
     */
    Transaction::Transaction(Transaction&& other) noexcept
    {
        _move_from(std::move(other));
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
            _move_from(std::move(other));

        return *this;
    }

    /**
     * @brief check if the transaction is active
     *
     * @return true
     * @return false
     */
    bool Transaction::is_active() const noexcept { return _is_active; }

    /**
     * @brief commit the transaction
     *
     */
    void Transaction::commit()
    {
        if (!_is_active || _db == nullptr)
            return;

        _db->execute("COMMIT;");
        _is_active = false;
    }

    /**
     * @brief rollback the transaction
     *
     */
    void Transaction::rollback()
    {
        if (!_is_active || _db == nullptr)
            return;

        _db->execute("ROLLBACK;");
        _is_active = false;
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
    void Transaction::_move_from(Transaction&& other) noexcept
    {
        _db        = other._db;
        _is_active = other._is_active;

        other._db        = nullptr;
        other._is_active = false;
    }

}   // namespace db
