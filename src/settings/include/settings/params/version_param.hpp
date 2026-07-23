#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__VERSION_PARAM_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__VERSION_PARAM_HPP__

#include "common/version.hpp"
#include "settings/params/param_core.hpp"

namespace settings
{

    using VersionParam = ParamCore<common::SemVer>;

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__VERSION_PARAM_HPP__
