#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
#define __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__

#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qwidget.h>

#include "settings/params/param_utils.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Creates an editor QWidget for any param type.
     *        The returned widget is un-parented — caller must parent it or add
     *        it to a layout (which takes ownership).
     *
     *        Supported param types:
     *          BoolParam       → QCheckBox
     *          StringParam     → QLineEdit
     *          NumericParam    → QSpinBox / QDoubleSpinBox
     *          NumericVecParam → N inline QSpinBox / QDoubleSpinBox (x/y/z
     * labels) EnumParam       → QComboBox VersionParam    → QLabel (read-only)
     */
    template <typename TParam>
    [[nodiscard]] QWidget* makeParamEditor(TParam& param)
    {
        using P = std::remove_cvref_t<TParam>;

        if constexpr (settings::is_bool_param<P>)
        {
            auto* cb = new QCheckBox();
            cb->setChecked(param.get());
            cb->setTristate(false);

            QObject::connect(
                cb,
                &QCheckBox::toggled,
                [&param](bool checked)
                {
                    param.set(checked);
                    param.commit();
                }
            );

            return cb;
        }
        else if constexpr (settings::is_string_param<P>)
        {
            auto* le = new QLineEdit();
            le->setText(QString::fromStdString(param.get()));
            le->setMinimumWidth(200);

            QObject::connect(
                le,
                &QLineEdit::editingFinished,
                [le, &param]()
                {
                    param.set(le->text().toStdString());
                    param.commit();
                }
            );

            return le;
        }
        else if constexpr (settings::is_numeric_param<P>)
        {
            using ValueType = typename P::value_type;

            if constexpr (std::floating_point<ValueType>)
            {
                auto* sb = utils::makeQChild<QDoubleSpinBox>();
                sb->setDecimals(4);
                sb->setRange(
                    param.getMinValue().value_or(
                        std::numeric_limits<ValueType>::lowest()
                    ),
                    param.getMaxValue().value_or(
                        std::numeric_limits<ValueType>::max()
                    )
                );
                sb->setValue(static_cast<double>(param.get()));

                QObject::connect(
                    sb,
                    &QDoubleSpinBox::editingFinished,
                    [sb, &param]()
                    {
                        param.set(static_cast<ValueType>(sb->value()));
                        param.commit();
                    }
                );

                return sb;
            }
            else
            {
                auto* sb = utils::makeQChild<QSpinBox>();
                sb->setRange(
                    static_cast<int>(param.getMinValue().value_or(
                        std::numeric_limits<int>::lowest()
                    )),
                    static_cast<int>(param.getMaxValue().value_or(
                        std::numeric_limits<int>::max()
                    ))
                );
                sb->setValue(static_cast<int>(param.get()));

                QObject::connect(
                    sb,
                    &QSpinBox::editingFinished,
                    [sb, &param]()
                    {
                        param.set(static_cast<ValueType>(sb->value()));
                        param.commit();
                    }
                );

                return sb;
            }
        }
        else if constexpr (settings::is_numeric_vec_param<P>)
        {
            using ValueType         = typename P::value_type;
            constexpr std::size_t N = P::size;

            auto* container = utils::makeQChild<QWidget>();
            auto* layout    = utils::makeQChild<QHBoxLayout>(container);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(6);

            constexpr std::array<const char*, 3> xyzLabels = {"x", "y", "z"};

            auto makeComponentEditor =
                [&](std::size_t i, auto getter, auto setter)
            {
                const char* labelText = (N <= 3) ? xyzLabels[i] : nullptr;

                if (labelText != nullptr)
                {
                    auto* lbl = new QLabel(QString(labelText));
                    lbl->setStyleSheet("color: #6a7a8a; font-size: 11px;");
                    layout->addWidget(lbl);
                }

                if constexpr (std::floating_point<ValueType>)
                {
                    auto* sb = utils::makeQChild<QDoubleSpinBox>();
                    sb->setDecimals(4);
                    sb->setFixedWidth(90);
                    sb->setValue(static_cast<double>(getter()));

                    QObject::connect(
                        sb,
                        &QDoubleSpinBox::editingFinished,
                        [sb, setter, &param]()
                        {
                            setter(static_cast<ValueType>(sb->value()));
                            param.commit();
                        }
                    );

                    layout->addWidget(sb);
                }
                else
                {
                    auto* sb = utils::makeQChild<QSpinBox>();
                    sb->setFixedWidth(80);
                    sb->setValue(static_cast<int>(getter()));

                    QObject::connect(
                        sb,
                        &QSpinBox::editingFinished,
                        [sb, setter, &param]()
                        {
                            setter(static_cast<ValueType>(sb->value()));
                            param.commit();
                        }
                    );

                    layout->addWidget(sb);
                }
            };

            // Unroll compile-time indices
            [&]<std::size_t... Is>(std::index_sequence<Is...>)
            {
                (makeComponentEditor(
                     Is,
                     [&param]() -> const ValueType& { return param.get(Is); },
                     [&param](ValueType v) { param.set(Is, v); }
                 ),
                 ...);
            }(std::make_index_sequence<N>{});

            layout->addStretch();
            return container;
        }
        else if constexpr (settings::is_enum_param<P>)
        {
            auto* cb = utils::makeQChild<QComboBox>();

            const auto& entries =
                P::entries();   // expected static method on EnumParam
            for (const auto& entry : entries)
                cb->addItem(QString::fromStdString(entry.label));

            // Set current index from current value
            const auto currentValue = param.get();
            for (int i = 0; i < static_cast<int>(entries.size()); ++i)
            {
                if (entries[static_cast<std::size_t>(i)].value == currentValue)
                {
                    cb->setCurrentIndex(i);
                    break;
                }
            }

            QObject::connect(
                cb,
                &QComboBox::currentIndexChanged,
                [cb, &param, entries](int idx)
                {
                    if (idx >= 0 && idx < static_cast<int>(entries.size()))
                    {
                        param.set(entries[static_cast<std::size_t>(idx)].value);
                        param.commit();
                    }
                }
            );

            return cb;
        }
        else if constexpr (settings::is_version_param<P>)
        {
            auto* lbl =
                new QLabel(QString::fromStdString(param.get().toString()));
            lbl->setStyleSheet(
                "color: #4a5a6a; font-family: monospace; font-size: 12px;"
            );
            return lbl;
        }

        else
        {
            // Fallback for unknown param types — render key as placeholder
            auto* lbl =
                new QLabel(QString("[unsupported: %1]")
                               .arg(QString::fromStdString(param.getKey())));
            lbl->setStyleSheet("color: #aa4444;");
            return lbl;
        }
    }

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
