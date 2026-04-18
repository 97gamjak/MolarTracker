#ifndef __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_HPP__
#define __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_HPP__

#include <QWidget>

namespace ui
{
    template <typename T>
    std::pair<T*, QWidget*> createLineEditWithLabel(QWidget* parent);

}   // namespace ui

#ifndef __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_TPP__
#include "validators.tpp"
#endif

#endif   // __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_HPP__