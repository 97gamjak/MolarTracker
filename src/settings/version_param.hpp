#ifndef __SETTINGS__VERSION_PARAM_HPP__
#define __SETTINGS__VERSION_PARAM_HPP__

#include "param_core.hpp"
#include "utils/version.hpp"

namespace settings
{

    using VersionParam = ParamCore<utils::SemVer>;

}   // namespace settings

#endif   // __SETTINGS__VERSION_PARAM_HPP__
