
#ifndef __COMMON__INCLUDE__COMMON__QT_HELPERS_HPP__
#define __COMMON__INCLUDE__COMMON__QT_HELPERS_HPP__

#include <QtContainerFwd>
#include <mstd/type_traits/enum_traits.hpp>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "common/container/set.hpp"

class QDialog;   // Forward declaration
class QWidget;   // Forward declaration

namespace common
{
    [[nodiscard]]
    QStringList toQStringList(const std::span<const std::string_view>& vec);
    [[nodiscard]]
    QStringList toQStringList(const std::span<std::string>& vec);

    [[nodiscard]]
    std::vector<QString> toQStringVector(const std::span<std::string>& vec);
    [[nodiscard]]
    std::vector<QString> toQStringVector(
        const std::span<const std::string>& vec
    );
    [[nodiscard]]
    Set<QString> toQStringSet(const Set<std::string>& vec);

    template <mstd::has_enum_meta EnumMeta>
    [[nodiscard]]
    QString toQString(EnumMeta value);

    void moveDialogToParentScreenCenter(QDialog* dlg, QWidget* parent);

    [[nodiscard]]
    QString toKBString(std::size_t bytes);

    template <typename T, typename... Args>
    [[nodiscard]]
    T* makeQChild(Args&&... args);

}   // namespace common

#ifndef __UTILS__INCLUDE__UTILS__QT_HELPERS_TPP__
#include "qt_helpers.tpp"   // IWYU pragma: export
#endif

#endif   // __COMMON__INCLUDE__COMMON__QT_HELPERS_HPP__