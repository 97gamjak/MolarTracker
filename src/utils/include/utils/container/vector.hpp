#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__

#include <vector>

#include "iterable.hpp"

template <typename T>
class Vector : public Iterable<std::vector<T>>
{
   private:
    using Iterable<std::vector<T>>::_items;

   public:
    using Iterable<std::vector<T>>::Iterable;

    const T& operator[](std::size_t index) const;

    void add(const T& item);
    void add(const std::vector<T>& items);
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__
#include "vector.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__