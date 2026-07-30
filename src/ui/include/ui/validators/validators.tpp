#ifndef __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_TPP__
#define __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_TPP__

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "common/qt_helpers.hpp"
#include "validators.hpp"

namespace ui
{
    template <typename T>
    std::pair<T*, QWidget*> createLineEditWithLabel(QWidget* parent)
    {
        auto* errorLabel = common::makeQChild<QLabel>(parent);
        auto* lineEdit   = common::makeQChild<T>(parent);
        lineEdit->attachErrorLabel(errorLabel);

        auto* container = common::makeQChild<QWidget>(parent);
        auto* layout    = common::makeQChild<QVBoxLayout>(container);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(2);
        layout->addWidget(lineEdit);
        layout->addWidget(errorLabel);

        return {lineEdit, container};
    }
}   // namespace ui

#endif   // __UI__INCLUDE__UI__VALIDATORS__VALIDATORS_TPP__