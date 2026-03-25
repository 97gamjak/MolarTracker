#ifndef __CONNECTIONS__CONNECTION_HPP__
#define __CONNECTIONS__CONNECTION_HPP__

#include <cstddef>
#include <optional>

// ============================================================================
// Connection
//  - Move-only RAII handle for a single subscription.
//  - Stores a small token: (owner pointer, subscription id, disconnect
//    function).
//
//
// Requirements on the producer (e.g. Param<T>):
//  - disconnectFunc(owner, id) must be valid, and idempotent.
// ============================================================================

/**
 * @brief Token representing a subscription connection, this struct contains the
 * owner pointer, subscription id, and disconnect function for a subscription,
 * this is used by the Connection class to manage the subscription and to
 * disconnect when the Connection object is destroyed or when disconnect() is
 * called on it
 *
 */
struct ConnectionToken
{
   public:
    /// Function pointer type for the disconnect function
    using DisconnectFn = void (*)(void*, std::size_t);

    /// Pointer to the owner of the subscription
    void* owner{};

    /// Subscription id
    std::size_t id{};

    /// Disconnect function for the subscription, this is a function pointer
    /// that takes the owner pointer and subscription id as arguments and is
    /// responsible for disconnecting the subscription when called
    DisconnectFn disconnectFunc{};

   public:
    void disconnect() const;
};

/**
 * @brief RAII handle for a single subscription.
 *
 * This class represents a connection to a subscription, it is move-only and
 * will automatically disconnect when it goes out of scope. It stores a small
 * token containing the owner pointer, subscription id, and disconnect function.
 *
 */
class Connection
{
   private:
    /// Optional token representing the subscription connection
    std::optional<ConnectionToken> _token;

   public:
    Connection() = default;

    Connection(const Connection&)            = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&& other) noexcept;
    Connection& operator=(Connection&& other) noexcept;

    ~Connection();

    [[nodiscard]] bool connected() const;
    explicit           operator bool() const;

    void reset();

    static Connection make(
        void*                                owner,
        std::size_t                          id,
        const ConnectionToken::DisconnectFn& func
    );
};

#endif   // __CONNECTIONS__CONNECTION_HPP__
