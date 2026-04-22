#include "ui/settings/param_editor.hpp"

namespace ui
{
    void onDirtyStripe(void* userData, const bool& isDirty)
    {
        auto* stripe = static_cast<QWidget*>(userData);
        stripe->setProperty("dirty", isDirty);
        stripe->style()->unpolish(stripe);
        stripe->style()->polish(stripe);
    }

    QWidget* makeBoolEditor(settings::BoolParam& param)
    {
        auto* checkBox = utils::makeQChild<QCheckBox>();
        checkBox->setChecked(param.get());
        checkBox->setTristate(false);

        QObject::connect(
            checkBox,
            &QCheckBox::toggled,
            [&param](bool checked) { param.set(checked); }
        );

        return checkBox;
    }

    QWidget* makeStringEditor(settings::StringParam& param)
    {
        constexpr size_t minimumWidth = 200;

        auto* lineEdit = utils::makeQChild<QLineEdit>();
        lineEdit->setText(QString::fromStdString(param.get()));
        lineEdit->setMinimumWidth(minimumWidth);

        QObject::connect(
            lineEdit,
            &QLineEdit::editingFinished,
            [lineEdit, &param]() { param.set(lineEdit->text().toStdString()); }
        );

        return lineEdit;
    }
}   // namespace ui