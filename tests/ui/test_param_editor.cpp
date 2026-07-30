#include <gtest/gtest.h>

#include <QDoubleSpinBox>
#include <QSpinBox>

#include "settings/params/numeric_param.hpp"
#include "ui/settings/param_editor.hpp"

namespace
{
    TEST(ParamEditor, IntNumericEditorPersistsValueOnEditingFinished)
    {
        settings::NumericParam<int> param("k", "T", "D");
        param.setDefault(0);
        auto* widget  = ui::makeParamEditor(param);
        auto* spinBox = qobject_cast<QSpinBox*>(widget);
        ASSERT_NE(spinBox, nullptr);

        const auto newValue = 42;
        spinBox->setValue(newValue);
        spinBox->editingFinished();

        EXPECT_EQ(param.get(), newValue);

        delete widget;
    }

    TEST(ParamEditor, DoubleNumericEditorPersistsValueOnEditingFinished)
    {
        settings::NumericParam<double> param("k", "T", "D");
        param.setDefault(0.0);
        auto* widget  = ui::makeParamEditor(param);
        auto* spinBox = qobject_cast<QDoubleSpinBox*>(widget);
        ASSERT_NE(spinBox, nullptr);

        const auto newValue = 3.5;
        spinBox->setValue(newValue);
        spinBox->editingFinished();

        EXPECT_DOUBLE_EQ(param.get(), newValue);

        delete widget;
    }

    TEST(ParamEditor, NumericEditorPersistsAcrossRepeatedEdits)
    {
        settings::NumericParam<int> param("k", "T", "D");
        param.setDefault(0);
        auto* widget  = ui::makeParamEditor(param);
        auto* spinBox = qobject_cast<QSpinBox*>(widget);
        ASSERT_NE(spinBox, nullptr);

        spinBox->setValue(1);
        spinBox->editingFinished();
        EXPECT_EQ(param.get(), 1);

        spinBox->setValue(2);
        spinBox->editingFinished();
        EXPECT_EQ(param.get(), 2);

        delete widget;
    }
}   // namespace
