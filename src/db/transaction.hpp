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
        bool      _is_active{false};

       public:
        explicit Transaction(Database& db, bool immediate = true);
        ~Transaction() noexcept;

        Transaction(Transaction const&)            = delete;
        Transaction& operator=(Transaction const&) = delete;

        Transaction(Transaction&& other) noexcept;
        Transaction& operator=(Transaction&& other) noexcept;

        [[nodiscard]] bool is_active() const noexcept;

        void commit();
        void rollback();

       private:   // PRIVATE HELPER METHODS
        void _move_from(Transaction&& other) noexcept;
    };
}   // namespace db

#endif   // __DB__TRANSACTION_HPP__