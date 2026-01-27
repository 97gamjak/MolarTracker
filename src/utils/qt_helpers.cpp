#include "qt_helpers.hpp"

#include <QStringList>

namespace utils
{
    QStringList toQStringList(const std::span<const std::string_view> vec)
    {
        QStringList list;
        for (const auto& str : vec)
            list.append(QString::fromStdString(std::string{str}));

        return list;
    }

}   // namespace utils