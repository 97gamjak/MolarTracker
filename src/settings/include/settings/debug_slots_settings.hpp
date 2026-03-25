#ifndef __SETTINGS__DEBUG_SLOTS_SETTINGS_HPP__
#define __SETTINGS__DEBUG_SLOTS_SETTINGS_HPP__

#include <string>
#include <utility>

#include "settings/params/params.hpp"

namespace settings
{
    /**
     * @brief Schema for the JSON representation of the debug slots settings
     *
     */
    class DebugSlotsSettingsSchema
    {
       public:
        /// key for the debug slots settings in JSON
        static constexpr const char* KEY = "debugSlotsSettings";
        /// title for the debug slots settings
        static constexpr const char* TITLE = "Debug Slots Settings";
        /// description for the debug slots settings
        static constexpr const char* DESCRIPTION =
            "Settings related to the debug slots dialog.";

        /***************
         * Window Size *
         ***************/

        /// key for the window size setting in JSON
        static constexpr const char* WINDOW_SIZE_KEY = "windowSize";
        /// title for the window size setting
        static constexpr const char* WINDOW_SIZE_TITLE = "Window Size";
        /// description for the window size setting
        static constexpr const char* WINDOW_SIZE_DESC =
            "The size of the debug slots dialog window in pixels, represented "
            "as a "
            "tuple of (width, height). For example, (800, 600) would represent "
            "a "
            "dialog that is 800 pixels wide and 600 pixels tall.";
        /// default value for the window size setting
        static constexpr const std::pair<int, int> WINDOW_SIZE_DEFAULT = {
            800,
            600
        };
    };

    /**
     * @brief Settings for the debug slots dialog, this class contains all the
     * parameters related to the debug slots dialog, such as the log level for
     * each slot, whether to apply changes to child slots, etc.
     *
     */
    class DebugSlotsSettings : public ParamContainerMixin<DebugSlotsSettings>
    {
       private:
        /// Schema for the JSON representation of the settings
        using Schema = DebugSlotsSettingsSchema;

        /// friend declaration for the ParamContainerMixin to access the private
        /// members of DebugSlotsSettings, this is necessary for the
        /// ParamContainerMixin to be able to access the _core member of
        /// DebugSlotsSettings to implement the functionality for the settings
        friend class ParamContainerMixin<DebugSlotsSettings>;

        /// The core container for the debug slots settings parameters
        ParamContainer _core{Schema::KEY, Schema::TITLE, Schema::DESCRIPTION};

        /// The size of the debug slots dialog window in pixels, represented as
        /// a tuple of (width, height)
        Numeric2DParam<int> _windowSize{
            Schema::WINDOW_SIZE_KEY,
            Schema::WINDOW_SIZE_TITLE,
            Schema::WINDOW_SIZE_DESC
        };

       public:
        DebugSlotsSettings();

        [[nodiscard]] std::pair<int, int> getWindowSize() const;

       private:
        template <typename Func>
        void _forEachParam(Func&& func) const;
        template <typename Func>
        void _forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__DEBUG_SLOTS_SETTINGS_TPP__
#include "debug_slots_settings.tpp"   // IWYU pragma: keep
#endif

#endif   // __SETTINGS__DEBUG_SLOTS_SETTINGS_HPP__