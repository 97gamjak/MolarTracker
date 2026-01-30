#ifndef __DB__TRANSACTION_HPP__
#define __DB__TRANSACTION_HPP__

namespace db
{
    class Database;

    /**
     * @brief A wrapper around an SQLite database transaction
     *
     */
    class Transaction
    {
       private:
        Database* _db{nullptr};
        bool      _isActive{false};

       public:
        explicit Transaction(Database& db, bool immediate = true);
        ~Transaction();

        Transaction(Transaction const&)            = delete;
        Transaction& operator=(Transaction const&) = delete;

        Transaction(Transaction&& other);
        Transaction& operator=(Transaction&& other);

        [[nodiscard]] bool isActive() const;

        void commit();
        void rollback();

       private:   // PRIVATE HELPER METHODS
        void _moveFrom(Transaction&& other);
    };
}   // namespace db

#endif   // __DB__TRANSACTION_HPP__