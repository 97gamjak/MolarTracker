#include "general_settings.hpp"

#include "config/constants.hpp"

namespace settings
{
    GeneralSettings::GeneralSettings()
        : _core{
              Schema::GENERAL_SETTINGS_KEY,
              Schema::GENERAL_SETTINGS_TITLE,
              Schema::GENERAL_SETTINGS_DESCRIPTION
          }
    {
        _version.set(utils::SemVer(Constants::getVersion()));
        _currentVersion = _version.get();
    }
}   // namespace settings