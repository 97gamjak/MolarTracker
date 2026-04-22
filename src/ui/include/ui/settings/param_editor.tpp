#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
#define __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__

#include <qabstractspinbox.h>
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
    template <typename TSpinBox, typename T>
    void makeNumericEditorHelper(TSpinBox* spinBox, const T& param)
    {
        using type = std::conditional_t<
            std::is_floating_point_v<typename T::value_type>,
            double,
            int>;

        const auto min = static_cast<type>(
            param.getMinValue().value_or(std::numeric_limits<type>::lowest())
        );

        const auto max = static_cast<type>(
            param.getMaxValue().value_or(std::numeric_limits<type>::max())
        );

        spinBox->setRange(min, max);

        spinBox->setValue(static_cast<type>(param.get()));
    }

    template <typename TSpinBox, typename T>
    void makeNumericEditorEditing(TSpinBox* spinBox, T& param)
    {
        QObject::connect(
            spinBox,
            &TSpinBox::editingFinished,
            [spinBox, &param]()
            {
                const auto result = param.set(
                    static_cast<typename T::value_type>(spinBox->value())
                );

                using type = std::conditional_t<
                    std::is_floating_point_v<typename T::value_type>,
                    double,
                    int>;

                if (!result.has_value())
                    spinBox->setValue(static_cast<type>(param.get()));
            }
        );
    }

    template <typename T>
    QWidget* makeNumericEditor(T& param)
    {
        using ValueType = T::value_type;

        if constexpr (std::floating_point<ValueType>)
        {
            auto* spinBox = utils::makeQChild<QDoubleSpinBox>();
            spinBox->setDecimals(
                static_cast<int>(param.getPrecision().value_or(4))
            );
            makeNumericEditorHelper(spinBox, param);

            QObject::connect(
                spinBox,
                &QDoubleSpinBox::editingFinished,
                [spinBox, &param]()
                { makeNumericEditorEditing(spinBox, param); }
            );

            return spinBox;
        }
        else
        {
            auto* spinBox = utils::makeQChild<QSpinBox>();
            makeNumericEditorHelper(spinBox, param);

            QObject::connect(
                spinBox,
                &QSpinBox::editingFinished,
                [spinBox, &param]()
                { makeNumericEditorEditing(spinBox, param); }
            );

            return spinBox;
        }
    }

    template <typename T>
    QWidget* makeNumericVecEditor(T& param)
    {
        constexpr std::size_t spacing         = 6;
        constexpr std::size_t numberOfEntries = T::size;

        auto* container = utils::makeQChild<QWidget>();
        auto* layout    = utils::makeQChild<QHBoxLayout>(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(spacing);

        const std::vector<const char*> xyzLabels = {"x", "y", "z"};

        auto makeComponentEditor = [&](std::size_t index)
        {
            const char* labelText =
                (numberOfEntries <= 3) ? xyzLabels[index] : nullptr;

            if (labelText != nullptr)
            {
                auto* lbl = new QLabel(QString(labelText));
                lbl->setStyleSheet("color: #6a7a8a; font-size: 11px;");
                layout->addWidget(lbl);
            }

            layout->addWidget(makeNumericEditor(param.getParam(index)));
        };

        // Unroll compile-time indices
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (makeComponentEditor(Is), ...);
        }(std::make_index_sequence<numberOfEntries>{});

        layout->addStretch();
        return container;
    }

    template <typename T>
    QWidget* makeEnumEditor(T& param)
    {
        auto* comboBox = utils::makeQChild<QComboBox>();

        const auto& entries = T::EnumMeta::values;

        for (const auto& entry : entries)
            comboBox->addItem(
                QString::fromStdString(T::EnumMeta::toString(entry))
            );

        // Set current index from current value
        const auto currentValue = param.get();
        const auto index        = T::EnumMeta::index(currentValue);

        if (index.has_value())
            comboBox->setCurrentIndex(static_cast<int>(index.value()));

        QObject::connect(
            comboBox,
            &QComboBox::currentIndexChanged,
            [comboBox, &param, entries](int idx)
            {
                if (idx >= 0 && idx < static_cast<int>(entries.size()))
                {
                    param.set(entries[static_cast<std::size_t>(idx)]);
                }
            }
        );

        return comboBox;
    }

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
    QWidget* makeParamEditor(TParam& param)
    {
        using P = std::remove_cvref_t<TParam>;

        if constexpr (settings::is_bool_param<P>)
            return makeBoolEditor(param);
        else if constexpr (settings::is_string_param<P>)
            return makeStringEditor(param);
        else if constexpr (settings::is_numeric_param<P>)
        {
            return makeNumericEditor(param);
        }
        else if constexpr (settings::is_numeric_vec_param<P>)
        {
            return makeNumericVecEditor(param);
        }
        else if constexpr (settings::is_enum_param<P>)
        {
            return makeEnumEditor(param);
        }
        else if constexpr (settings::is_version_param<P>)
        {
            auto* lbl = utils::makeQChild<QLabel>(
                QString::fromStdString(param.get().toString())
            );

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
            auto* group = utils::makeQChild<QGroupBox>(
                QString::fromStdString(param.getTitle())
            );
            auto* groupLayout = utils::makeQChild<QFormLayout>(group);

            constexpr std::array<int, 4> margins = {12, 8, 12, 8};

            groupLayout->setContentsMargins(
                margins[0],
                margins[1],
                margins[2],
                margins[3]
            );
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
            auto* rowWidget = utils::makeQChild<QWidget>();
            rowWidget->setObjectName("paramRow");
            auto* rowLayout = utils::makeQChild<QHBoxLayout>(rowWidget);

            constexpr std::array<int, 4> margins = {0, 6, 0, 6};
            constexpr std::size_t        spacing = 8;
            rowLayout->setContentsMargins(
                margins[0],
                margins[1],
                margins[2],
                margins[3]
            );
            rowLayout->setSpacing(spacing);

            auto* dirtyStripe = utils::makeQChild<QWidget>();

            constexpr std::pair<int, int> dirtyStripeSize = {3, 20};
            dirtyStripe->setFixedSize(
                dirtyStripeSize.first,
                dirtyStripeSize.second
            );
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

            auto* label = utils::makeQChild<QLabel>(
                QString::fromStdString(param.getTitle())
            );
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
