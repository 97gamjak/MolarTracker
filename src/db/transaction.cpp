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
        : _db(&db), _isActive(true)
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
    Transaction::~Transaction()
    {
        if (_isActive && _db != nullptr)
        {
            try
            {
                rollback();
            }
            catch (...)
            {
            }
        }
    }

    /**
     * @brief Move constructor
     *
     * @param other
     */
    Transaction::Transaction(Transaction&& other)
    {
        _moveFrom(std::move(other));
    }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Transaction&
     */
    Transaction& Transaction::operator=(Transaction&& other)
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
     */
    void Transaction::commit()
    {
        if (!_isActive || _db == nullptr)
            return;

        _db->execute("COMMIT;");
        _isActive = false;
    }

    /**
     * @brief rollback the transaction
     *
     */
    void Transaction::rollback()
    {
        if (!_isActive || _db == nullptr)
            return;

        _db->execute("ROLLBACK;");
        _isActive = false;
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
    void Transaction::_moveFrom(Transaction&& other)
    {
        _db       = other._db;
        _isActive = other._isActive;

        other._db       = nullptr;
        other._isActive = false;
    }

}   // namespace db
