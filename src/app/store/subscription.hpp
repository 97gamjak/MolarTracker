#ifndef __APP__STORE__SUBSCRIPTION_HPP__
#define __APP__STORE__SUBSCRIPTION_HPP__

#include <cstddef>
#include <functional>
#include <utility>

namespace app
{

    class Subscription final
    {
       private:
        std::function<void(std::size_t)> _unsubscribe;
        std::size_t                      _id = 0;

       public:
        Subscription() = default;

        explicit Subscription(
            std::function<void(std::size_t)> unsubscribeFunction,
            std::size_t                      id
        )
            : _unsubscribe{std::move(unsubscribeFunction)}, _id{id}
        {
        }

        ~Subscription() { reset(); }

        Subscription(const Subscription&)            = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other)
            : _unsubscribe{std::move(other._unsubscribe)}, _id{other._id}
        {
            other._id = 0;
        }

        Subscription& operator=(Subscription&& other)
        {
            if (this == &other)
                return *this;

            reset();
            _unsubscribe = std::move(other._unsubscribe);
            _id          = other._id;
            other._id    = 0;
            return *this;
        }

        void reset()
        {
            if (_unsubscribe)
            {
                _unsubscribe(_id);
                _unsubscribe = nullptr;
            }
        }
    };

}   // namespace app

#endif   // __APP__STORE__SUBSCRIPTION_HPP__