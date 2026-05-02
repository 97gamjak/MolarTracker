#ifndef __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__
#define __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__

#include <QtContainerFwd>
#include <mstd/type_traits/enum_traits.hpp>
#include <span>
#include <string_view>
#include <vector>

class QDialog;   // Forward declaration
class QWidget;   // Forward declaration

namespace utils
{
    QStringList toQStringList(const std::span<const std::string_view>& vec);
    QStringList toQStringList(const std::span<std::string>& vec);

    std::vector<QString> toQStringVector(const std::span<std::string>& vec);

    template <mstd::has_enum_meta EnumMeta>
    QString toQString(EnumMeta value);

    void moveDialogToParentScreenCenter(QDialog* dlg, QWidget* parent);

    template <typename T, typename... Args>
    T* makeQChild(Args&&... args);

}   // namespace utils

#ifndef __UTILS__INCLUDE__UTILS__QT_HELPERS_TPP__
#include "qt_helpers.tpp"   // IWYU pragma: export
#endif

#endif   // __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__