#ifndef __CONNECTIONS__SIGNAL_HPP__
#define __CONNECTIONS__SIGNAL_HPP__

#include <cstddef>
#include <unordered_map>

class Connection;   // Forward declaration

/**
 * @brief A simple signal-slot implementation for event handling, this class
 * allows subscribers to connect callback functions that will be called when the
 * signal is emitted. The subscribers can also disconnect from the signal using
 * the Connection object returned by the connect() method.
 *
 * @tparam Tag A type that represents the type of data that will be passed to
 * the subscribers when the signal is emitted, this can be any type that is
 * copyable and can be passed as a const reference to the callback functions.
 */
template <typename Tag>
class Signal
{
   public:
    /// Type alias for the type of data that will be passed to the subscribers
    using TagType = typename Tag::TagType;

    /// Type alias for the callback function, this is a function pointer that
    /// takes a pointer to the user data and a const reference to the data
    /// associated with the signal, this is used to define the type of the
    /// callback functions that can be connected to the signal
    using CallbackFn = void (*)(void*, const TagType&);

   private:
    /**
     * @brief Struct representing a subscriber to the signal
     *
     */
    struct Subscriber
    {
        /// The callback function to call when the signal is emitted
        CallbackFn func{};
        /// A user-defined pointer that will be passed to the callback function
        void* user{};
    };

    /// Counter for generating unique subscriber IDs
    std::size_t _idCounter{0};

    /// Map of subscriber IDs to Subscriber structs
    std::unordered_map<std::size_t, Subscriber> _subscribers;

   public:
    Connection connect(CallbackFn func, void* user);

    void notify(const TagType& arg) const;

   private:
    static void _disconnect(void* owner, std::size_t id);
};

#ifndef __CONNECTIONS__SIGNAL_TPP__
#include "signal.tpp"
#endif   // __CONNECTIONS__SIGNAL_TPP__

#endif   // __CONNECTIONS__SIGNAL_HPP__