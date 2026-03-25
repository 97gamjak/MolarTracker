#ifndef __APP__INCLUDE__APP__STORE__BASE__STORE_STATE_HPP__
#define __APP__INCLUDE__APP__STORE__BASE__STORE_STATE_HPP__

#include <cstdint>

namespace app
{

    /**
     * @brief State of an item in the store
     *
     */
    enum class StoreState : std::uint8_t
    {
        Clean,
        New,
        Modified,
        Deleted
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__BASE__STORE_STATE_HPP__