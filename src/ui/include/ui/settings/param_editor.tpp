#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
#define __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__

#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qwidget.h>

#include <QGroupBox>

#include "param_editor.hpp"
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
                        const auto result =
                            param.set(static_cast<ValueType>(sb->value()));

                        if (result.has_value())
                            param.commit();
                        else
                            sb->setValue(static_cast<int>(param.get()));
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
                        const auto result =
                            param.set(static_cast<ValueType>(sb->value()));

                        if (result.has_value())
                            param.commit();
                        else
                            sb->setValue(static_cast<int>(param.get()));
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

            auto makeComponentEditor = [&](std::size_t i, auto getter)
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
                        [sb, &param, i]()
                        {
                            const auto result = param.set(
                                i,
                                static_cast<ValueType>(sb->value())
                            );
                            if constexpr (requires { result.has_value(); })
                            {
                                if (!result.has_value())
                                {
                                    sb->setValue(
                                        static_cast<int>(param.get(i))
                                    );
                                    return;
                                }
                            }

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
                        [sb, &param, i]()
                        {
                            const auto result = param.set(
                                i,
                                static_cast<ValueType>(sb->value())
                            );
                            if constexpr (requires { result.has_value(); })
                            {
                                if (!result.has_value())
                                {
                                    sb->setValue(
                                        static_cast<int>(param.get(i))
                                    );
                                    return;
                                }
                            }

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
                     [&param]() -> const ValueType& { return param.get(Is); }
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
                P::EnumMeta::values;   // expected static method on EnumParam
            for (const auto& entry : entries)
                cb->addItem(
                    QString::fromStdString(P::EnumMeta::toString(entry))
                );

            // Set current index from current value
            const auto currentValue = param.get();
            const auto index        = P::EnumMeta::index(currentValue);

            if (index.has_value())
                cb->setCurrentIndex(static_cast<int>(index.value()));

            QObject::connect(
                cb,
                &QComboBox::currentIndexChanged,
                [cb, &param, entries](int idx)
                {
                    if (idx >= 0 && idx < static_cast<int>(entries.size()))
                    {
                        param.set(entries[static_cast<std::size_t>(idx)]);
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
            static_assert(
                std::is_same_v<P, void>,
                "Unsupported param type in makeParamEditor"
            );
            return nullptr;   // Unreachable, but silences compiler warning
        }
    }

    template <typename TParam>
    void buildParamRows(
        TParam&                  param,
        QFormLayout*             layout,
        std::vector<Connection>& connections,
        SectionMode              mode
    )
    {
        if constexpr (settings::IsParamContainer<TParam>)
        {
            // Sub-container → group box with its own form layout
            auto* group =
                new QGroupBox(QString::fromStdString(param.getTitle()));
            auto* groupLayout = new QFormLayout(group);
            groupLayout->setContentsMargins(12, 8, 12, 8);
            groupLayout->setSpacing(0);
            groupLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            param.forEachParam(
                [&](auto& subParam)
                { buildParamRows(subParam, groupLayout, connections, mode); }
            );

            // Span the group box across both columns
            layout->addRow(group);
        }
        else
        {
            if (mode == SectionMode::SubContainersOnly)
                return;   // Skip leaf params

            // Leaf param → normal editor row
            auto* rowWidget = new QWidget();
            rowWidget->setObjectName("paramRow");
            auto* rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(0, 6, 0, 6);
            rowLayout->setSpacing(8);

            auto* dirtyStripe = new QWidget();
            dirtyStripe->setFixedSize(3, 20);
            dirtyStripe->setObjectName("dirtyStripe");
            dirtyStripe->setProperty("dirty", false);
            rowLayout->addWidget(dirtyStripe);

            QWidget* editor = makeParamEditor(param);
            rowLayout->addWidget(editor);

            if constexpr (requires { param.isRebootRequired(); })
            {
                if (param.isRebootRequired())
                {
                    auto* rebootLabel = new QLabel("⟳ restart required");
                    rebootLabel->setObjectName("rebootBadge");
                    rowLayout->addWidget(rebootLabel);
                }
            }

            rowLayout->addStretch();

            auto* label = new QLabel(QString::fromStdString(param.getTitle()));
            label->setObjectName("paramLabel");
            label->setToolTip(QString::fromStdString(param.getDescription()));

            layout->addRow(label, rowWidget);

            // Dirty subscription
            auto connectDirty = [&](Connection conn)
            { connections.push_back(std::move(conn)); };

            if constexpr (requires {
                              {
                                  param.subscribeToDirty(nullptr, nullptr)
                              } -> std::same_as<Connection>;
                          })
            {
                connectDirty(param.subscribeToDirty(onDirtyStripe, dirtyStripe)
                );
            }
            else
            {
                for (auto& conn :
                     param.subscribeToDirty(onDirtyStripe, dirtyStripe))
                    connections.push_back(std::move(conn));
            }
        }
    }

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
