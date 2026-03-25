#ifndef __UI__INCLUDE__UI__LOGGING__CUSTOM_ROLES_HPP__
#define __UI__INCLUDE__UI__LOGGING__CUSTOM_ROLES_HPP__

#include <QVariant>
#include <cstdint>

namespace ui
{
    enum CustomRoles : uint16_t
    {
        IsCascadeApply = Qt::UserRole + 1,
    };

}   // namespace ui
#endif   // __UI__INCLUDE__UI__LOGGING__CUSTOM_ROLES_HPP__