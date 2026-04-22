#ifndef __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__
#define __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__

#include <functional>
#include <optional>

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
    using func = std::function<void(void*, const bool& isDirty)>;
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
    using func = std::function<void(void*)>;
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
    using func =
        std::function<void(void*, const std::optional<ProfileId>& profileId)>;
};

#endif   // __CONFIG__INCLUDE__CONFIG__SIGNAL_TAGS_HPP__