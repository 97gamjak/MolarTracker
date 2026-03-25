#ifndef __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__
#define __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__

#include <QtContainerFwd>
#include <span>
#include <string_view>

class QDialog;   // Forward declaration
class QWidget;   // Forward declaration

namespace utils
{
    QStringList toQStringList(const std::span<const std::string_view>& vec);

    void moveDialogToParentScreenCenter(QDialog* dlg, QWidget* parent);

    template <typename T, typename... Args>
    T* makeQChild(Args&&... args);

}   // namespace utils

#ifndef __UTILS__INCLUDE__UTILS__QT_HELPERS_TPP__
#include "qt_helpers.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__QT_HELPERS_HPP__