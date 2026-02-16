#include "connection.hpp"

#include <cstddef>
#include <optional>
#include <utility>

/**
 * @brief Move constructor for Connection, this transfers ownership of the
 * subscription from the other Connection object to this one, and leaves the
 * other Connection object in a valid but disconnected state
 *
 * @param other
 */
Connection::Connection(Connection&& other)
    : _token(std::exchange(other._token, std::nullopt))
{
}

/**
 * @brief Move assignment operator for Connection, this transfers ownership of
 * the subscription from the other Connection object to this one, and leaves the
 * other Connection object in a valid but disconnected state, if this Connection
 * object already has a subscription, it will be disconnected before taking
 * ownership of the new subscription
 *
 * @param other
 * @return Connection&
 */
Connection& Connection::operator=(Connection&& other)
{
    if (this != &other)
    {
        reset();
        _token = std::exchange(other._token, std::nullopt);
    }
    return *this;
}

/**
 * @brief Destructor for Connection, this will automatically disconnect the
 * subscription if it is still connected when the Connection object goes out of
 * scope
 */
Connection::~Connection() { reset(); }

/**
 * @brief Check if the Connection object is currently connected to a
 * subscription
 *
 * @return true if the Connection object is connected to a subscription, false
 * otherwise
 */
bool Connection::connected() const { return _token.has_value(); }

/**
 * @brief Explicit bool operator for Connection, this allows the Connection
 * object to be used in boolean contexts, it returns true if the Connection
 * object is connected to a subscription, and false otherwise
 *
 * @return true if the Connection object is connected to a subscription, false
 * otherwise
 */
Connection::operator bool() const { return connected(); }

/**
 * @brief Reset the Connection object, this will disconnect the subscription if
 * it is currently connected, and leave the Connection object in a valid but
 * disconnected state
 */
void Connection::reset()
{
    if (_token)
    {
        _token->disconnect();
        _token.reset();
    }
}

/**
 * @brief Disconnect the subscription associated with this ConnectionToken, this
 * is called by the Connection object when it is destroyed or when disconnect()
 * is called on it, it calls the disconnect function stored in the token with
 * the owner pointer and subscription id to perform the disconnection
 */
void ConnectionToken::disconnect() const { disconnect_fn(owner, id); }

/**
 * @brief Static factory method for creating a Connection object, this takes the
 * owner pointer, subscription id, and disconnect function for a subscription,
 * and returns a Connection object that manages the subscription, the disconnect
 * function must be valid and idempotent, meaning that it can be called multiple
 * times without causing any issues, this allows the Connection object to safely
 * call disconnect() in its destructor without worrying about whether the
 * subscription has already been disconnected or not
 *
 * @param owner A pointer representing the owner of the subscription, this can
 * be used to provide context for the disconnect function when it is called
 * @param id The subscription id, this is used by the disconnect function to
 * identify which subscription to disconnect when it is called
 * @param fn The disconnect function, this is a function pointer that takes the
 * owner pointer and subscription id as parameters and performs the
 * disconnection of the subscription when called, it must be valid and
 * idempotent
 * @return Connection An object representing the subscription connection, this
 * can be used to manage the subscription and will automatically disconnect when
 * it goes out of scope
 */
Connection Connection::make(void* owner, std::size_t id, DisconnectFn fn)
{
    Connection c;
    c._token = Token{owner, id, fn};
    return c;
}