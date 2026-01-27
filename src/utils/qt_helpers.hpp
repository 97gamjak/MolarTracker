#ifndef __UTILS__QT_HELPERS_HPP__
#define __UTILS__QT_HELPERS_HPP__

#include <QStringList>
#include <span>
#include <string_view>

namespace utils
{
    QStringList toQStringList(const std::span<const std::string_view> vec);
}   // namespace utils

#endif   // __UTILS__QT_HELPERS_HPP__