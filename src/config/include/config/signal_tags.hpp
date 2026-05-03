#ifndef __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__
#define __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__

#include <functional>
#include <optional>
#include <unordered_map>

#include "config/id_types.hpp"

/**
 * @brief Common signal tags used across the application, this file defines
 * common event tags that can be used for emitting and subscribing to events
 * across different components of the application, such as stores, settings, and
 * other observable objects. This helps to standardize the event tags and ensure
 * consistency in how events are emitted and handled throughout the application.
 *
 */
struct OnDirtyChanged
{
    /// Type alias for the change callback function for the dirty state
    using func = std::function<void(const bool& isDirty)>;
};

/**
 * @brief Signal tag for when a new item is added to a store, this can be used
 * to emit an event when a new item is added, allowing other parts of the
 * application to react to the addition of the item.
 *
 */
template <typename T>
struct OnStoreItemAdded
{
    /// Type alias for the add callback function
    using func = std::function<void(const std::vector<T>& item)>;
};

/**
 * @brief Signal tag for when an item is updated in a store, this can be used
 * to emit an event when an item is updated, allowing other parts of the
 * application to react to the update of the item.
 *
 */
template <typename T>
struct OnStoreItemUpdated
{
    /// Type alias for the update callback function
    using func = std::function<void(const std::vector<T>& item)>;
};

/**
 * @brief Signal tag for when an item is removed from a store, this can be used
 * to emit an event when an item is removed, allowing other parts of the
 * application to react to the removal of the item.
 *
 */
template <typename IdType>
struct OnStoreItemRemoved
{
    /// Type alias for the remove callback function
    using func = std::function<void(const std::vector<IdType>& item)>;
};

/**
 * @brief Signal tag for when an id is remapped, this can be used to emit an
 * event when an id is remapped, allowing other parts of the application to
 * react to the remapping of the id.
 *
 */
template <typename IdType>
struct OnIdRemap
{
    /// Type alias for the remap callback function
    using func = std::function<void(
        const std::unordered_map<IdType, IdType, typename IdType::Hash>& map
    )>;
};

/**
 * @brief Signal tag for when settings are saved, this can be used to emit an
 * event when the settings are saved, allowing other parts of the application to
 * react to the save event, such as updating the UI or saving changes to the
 * database.
 *
 */
struct OnSaved
{
    /// Type alias for the change callback function for when settings are saved
    using func = std::function<void()>;
};

/**
 * @brief Signal tag for profile changes, this can be used to emit events when a
 * profile is changed in the ProfileStore, allowing other parts of the
 * application to react to profile changes, such as updating the UI or saving
 * changes to the database.
 *
 */
struct OnProfileChanged
{
    /// Type alias for the change callback function for the dirty state
    using func = std::function<void(const std::optional<ProfileId>& profileId)>;
};

#endif   // __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__