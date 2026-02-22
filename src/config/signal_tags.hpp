#ifndef __CONFIG__SIGNAL_TAGS_HPP__
#define __CONFIG__SIGNAL_TAGS_HPP__

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
    /// Type alias for the type of data associated with the event
    using TagType = bool;

    /// Type alias for the change callback function for the dirty state
    using func = void (*)(void*, const bool& isDirty);
};

#endif   // __CONFIG__SIGNAL_TAGS_HPP__