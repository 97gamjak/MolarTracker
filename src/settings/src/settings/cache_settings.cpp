#include "settings/cache_settings.hpp"

namespace settings
{
    /**
     * @brief Construct a new Cache Settings:: Cache Settings object
     *
     */
    CacheSettings::CacheSettings()
    {
        _globalMaxCacheSize.setDefault(Schema::GLOBAL_MAX_CACHE_SIZE_DEFAULT);
        _globalMaxCacheSize.setMinValue(Schema::GLOBAL_MAX_CACHE_SIZE_MIN);
        _globalMaxCacheSize.setMaxValue(Schema::GLOBAL_MAX_CACHE_SIZE_MAX);
    }

    /**
     * @brief Get the global maximum cache size setting, representing the
     * maximum number of elements that can be cached across the application.
     *
     * @return NumericParam<std::size_t>& A reference to the global maximum
     * cache size setting.
     */
    NumericParam<std::size_t>& CacheSettings::getGlobalMaxCacheSize()
    {
        return _globalMaxCacheSize;
    }
}   // namespace settings