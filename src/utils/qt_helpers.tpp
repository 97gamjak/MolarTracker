#ifndef __UTILS__QT_HELPERS_TPP__
#define __UTILS__QT_HELPERS_TPP__

#include "qt_helpers.hpp"

namespace utils
{
    /**
     * @brief Create a new child widget of type T with the given arguments, this
     * is a helper function that can be used to create new Qt widgets as
     * children of a parent widget, this allows for more concise and readable
     * code when creating new widgets in the UI, and ensures that the widgets
     * are properly parented in the Qt object hierarchy.
     *
     * @tparam T The type of widget to create, this should be a subclass of
     * QWidget, and will be created as a child of the parent widget specified
     * in the arguments.
     * @param args The arguments to forward to the constructor of the widget,
     * this should include at least a pointer to the parent widget (e.g. a
     * QMainWindow or another QWidget), and can include any additional arguments
     * needed to construct the widget (e.g. text for a QLabel, or items for a
     * QComboBox).
     *
     * @return A pointer to the newly created widget of type T, this will be a
     * child of the parent widget specified in the arguments, and can be used to
     * interact with the widget after it has been created.
     */
    template <typename T, typename... Args>
    T* makeQChild(Args&&... args)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        return new T(std::forward<Args>(args)...);
    }

}   // namespace utils

#endif   // __UTILS__QT_HELPERS_TPP__