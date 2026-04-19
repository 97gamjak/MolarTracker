#include "ui/settings/settings_section.hpp"

#include <qlabel.h>
#include <qstyle.h>

#include "settings/params/param_utils.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    template <settings::IsParamContainer TSection>
    SettingsSection<TSection>::SettingsSection(TSection& section)
    {
        _build(section);
    }

    template <settings::IsParamContainer TSection>
    std::vector<Connection>& SettingsSection<TSection>::getConnections()
    {
        return _connections;
    }

    template <settings::IsParamContainer TSection>
    void SettingsSection<TSection>::_build(TSection& section)
    {
        auto* outerLayout = utils::makeQChild<QVBoxLayout>(this);
        outerLayout->setContentsMargins(0, 0, 0, 0);
        outerLayout->setSpacing(0);

        // ── Section header
        // ────────────────────────────────────────────────
        auto* header = utils::makeQChild<QWidget>(this);
        header->setObjectName("sectionHeader");
        auto* headerLayout = utils::makeQChild<QVBoxLayout>(header);
        headerLayout->setContentsMargins(20, 16, 20, 12);
        headerLayout->setSpacing(2);

        auto* titleLabel = utils::makeQChild<QLabel>(header);
        titleLabel->setText(QString::fromStdString(section.getTitle()));
        titleLabel->setObjectName("sectionTitle");

        auto* descLabel = utils::makeQChild<QLabel>(header);
        descLabel->setText(QString::fromStdString(section.getDescription()));
        descLabel->setObjectName("sectionDescription");
        descLabel->setWordWrap(true);

        headerLayout->addWidget(titleLabel);
        headerLayout->addWidget(descLabel);
        outerLayout->addWidget(header);

        // ── Divider
        // ───────────────────────────────────────────────────────
        auto* divider = utils::makeQChild<QFrame>(this);
        divider->setFrameShape(QFrame::HLine);
        divider->setObjectName("sectionDivider");
        outerLayout->addWidget(divider);

        // ── Params form
        // ───────────────────────────────────────────────────
        auto* scrollArea = utils::makeQChild<QScrollArea>(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        auto* formContainer = utils::makeQChild<QWidget>(scrollArea);
        auto* formLayout    = utils::makeQChild<QFormLayout>(formContainer);
        formLayout->setContentsMargins(20, 12, 20, 20);
        formLayout->setSpacing(0);
        formLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

        // ── Build one row per param
        // ───────────────────────────────────────
        section._forEachParam(
            [&](auto& param)
            {
                // Row container (label + editor + optional reboot warning)
                auto* rowWidget = utils::makeQChild<QWidget>(formContainer);
                rowWidget->setObjectName("paramRow");
                auto* rowLayout = utils::makeQChild<QHBoxLayout>(rowWidget);
                rowLayout->setContentsMargins(0, 6, 0, 6);
                rowLayout->setSpacing(8);

                // Dirty indicator stripe
                auto* dirtyStripe = utils::makeQChild<QWidget>(rowWidget);
                dirtyStripe->setFixedSize(3, 20);
                dirtyStripe->setObjectName("dirtyStripe");
                dirtyStripe->setProperty("dirty", false);
                rowLayout->addWidget(dirtyStripe);

                // Editor widget — type dispatched at compile time
                QWidget* editor = makeParamEditor(param);
                rowLayout->addWidget(editor);

                // Reboot required badge
                if (param.isRebootRequired())
                {
                    auto* rebootLabel =
                        utils::makeQChild<QLabel>("⟳ restart required");
                    rebootLabel->setObjectName("rebootBadge");
                    rowLayout->addWidget(rebootLabel);
                }

                rowLayout->addStretch();

                // Label for the form row
                auto* label = utils::makeQChild<QLabel>(
                    QString::fromStdString(param.getTitle())
                );
                label->setObjectName("paramLabel");
                label->setToolTip(
                    QString::fromStdString(param.getDescription())
                );

                formLayout->addRow(label, rowWidget);

                // ── Dirty subscription
                // ──────────────────────────────────────── NumericVecParam
                // returns vector<Connection>, others return Connection
                if constexpr (requires {
                                  {
                                      param.subscribeToDirty(nullptr, nullptr)
                                  } -> std::same_as<Connection>;
                              })
                {
                    _connections.push_back(param.subscribeToDirty(
                        [](void* userData, bool isDirty)
                        {
                            auto* stripe = static_cast<QWidget*>(userData);
                            stripe->setProperty("dirty", isDirty);
                            // Re-polish to pick up QSS property change
                            stripe->style()->unpolish(stripe);
                            stripe->style()->polish(stripe);
                        },
                        static_cast<void*>(dirtyStripe)
                    ));
                }
                else
                {
                    auto vecConnections = param.subscribeToDirty(
                        [](void* userData, bool isDirty)
                        {
                            auto* stripe = static_cast<QWidget*>(userData);
                            stripe->setProperty("dirty", isDirty);
                            stripe->style()->unpolish(stripe);
                            stripe->style()->polish(stripe);
                        },
                        static_cast<void*>(dirtyStripe)
                    );
                    for (auto& conn : vecConnections)
                        _connections.push_back(std::move(conn));
                }
            }
        );

        scrollArea->setWidget(formContainer);
        outerLayout->addWidget(scrollArea, 1);
    }

}   // namespace ui