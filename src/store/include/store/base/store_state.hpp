#ifndef __STORE__INCLUDE__STORE__BASE__STORE_STATE_HPP__
#define __STORE__INCLUDE__STORE__BASE__STORE_STATE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

namespace store
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STORE_STATE_LIST(X) \
    X(Clean)                \
    X(New)                  \
    X(Modified)             \
    X(Deleted)

    /**
     * @brief State of an item in the store
     *
     */
    MSTD_ENUM(StoreState, std::uint8_t, STORE_STATE_LIST)

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__BASE__STORE_STATE_HPP__