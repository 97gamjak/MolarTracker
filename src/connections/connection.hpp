#ifndef __CONNECTIONS__CONNECTION_HPP__
#define __CONNECTIONS__CONNECTION_HPP__

#include <cstddef>
#include <memory>
#include <optional>

// ============================================================================
// Connection
//  - Move-only RAII handle for a single subscription.
//  - Stores a small token: (owner pointer, subscription id, disconnect
//    function).
//
//
// Requirements on the producer (e.g. Param<T>):
//  - disconnect_fn(owner, id) must be valid, and idempotent.
// ============================================================================

/**
 * @brief Token representing a subscription connection, this struct contains the
 * owner pointer, subscription id, and disconnect function for a subscription,
 * this is used by the Connection class to manage the subscription and to
 * disconnect when the Connection object is destroyed or when disconnect() is
 * called on it
 */
struct ConnectionToken
{
    using DisconnectFn = void (*)(void*, std::size_t);

    void*        owner{};
    std::size_t  id{};
    DisconnectFn disconnect_fn{};

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
    using Token = ConnectionToken;

   private:
    std::optional<Token> _token;

   public:
    using DisconnectFn = Token::DisconnectFn;

    Connection() = default;

    Connection(const Connection&)            = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&& other);

    Connection& operator=(Connection&& other);

    ~Connection();

    [[nodiscard]] bool connected() const;
    explicit           operator bool() const;

    void reset();

    static Connection make(void* owner, std::size_t id, DisconnectFn fn);
};

#endif   // __CONNECTIONS__CONNECTION_HPP__
