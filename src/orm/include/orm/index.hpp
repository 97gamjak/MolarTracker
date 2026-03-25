#ifndef __ORM__INDEX_HPP__
#define __ORM__INDEX_HPP__

#include "config/strong_id.hpp"

namespace orm
{

    struct ColumnIndexTag
    {
    };
    struct BindIndexTag
    {
    };

    using ColumnIndex = StrongId<ColumnIndexTag, int>;
    using BindIndex   = StrongId<BindIndexTag, int>;

    constexpr ColumnIndex columnIndex(int index) { return ColumnIndex{index}; }
    constexpr ColumnIndex columnIndex(std::size_t index)
    {
        return ColumnIndex{static_cast<int>(index)};
    }

    constexpr BindIndex bindIndex(int index) { return BindIndex{index + 1}; }
    constexpr BindIndex bindIndex(std::size_t index)
    {
        return BindIndex{static_cast<int>(index) + 1};
    }

}   // namespace orm

#endif   // __ORM__INDEX_HPP__