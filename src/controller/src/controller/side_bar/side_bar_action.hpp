#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_ACTION_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_ACTION_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

#define SIDE_BAR_ACTIONS(X) \
    X(ItemClicked)          \
    X(ItemDoubleClicked)    \
    X(ContextMenuRequested)

MSTD_ENUM(SideBarAction, std::uint8_t, SIDE_BAR_ACTIONS)

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_ACTION_HPP__
