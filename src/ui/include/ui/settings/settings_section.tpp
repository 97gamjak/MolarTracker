#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_TPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_TPP__

#include <qlabel.h>
#include <qstyle.h>

#include "settings/params/param_utils.hpp"
#include "ui/settings/param_editor.hpp"
#include "ui/settings/settings_section.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Settings Section< T Section>:: Settings Section
     * object
     *
     * @tparam TSection
     * @param section
     * @param mode
     */
    template <settings::IsParamContainer TSection>
    SettingsSection<TSection>::SettingsSection(
        TSection&   section,
        SectionMode mode
    )
    {
        _build(section, mode);
    }

    /**
     * @brief get the connections for the section
     *
     * @tparam TSection
     * @return std::vector<Connection>&
     */
    template <settings::IsParamContainer TSection>
    std::vector<Connection>& SettingsSection<TSection>::getConnections()
    {
        return _connections;
    }

    /**
     * @brief build the UI for the settings section, this function creates the
     * header and the form layout for the section, it also builds one row per
     * parameter in the section by calling buildParamRows for each parameter,
     * which creates the appropriate editor widget for each parameter type and
     * adds it to the form layout, the connections for each parameter editor are
     * stored in the _connections vector for later cleanup
     *
     * @tparam TSection
     * @param section The ParamContainer section to build the UI for
     * @param mode The SectionMode to determine which parameters to show (leaves
     * only, subcontainers only, or all)
     */
    template <settings::IsParamContainer TSection>
    void SettingsSection<TSection>::_build(TSection& section, SectionMode mode)
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
        section.forEachParam(
            [&](auto& param)
            { buildParamRows(param, formLayout, _connections, mode); }
        );

        scrollArea->setWidget(formContainer);
        outerLayout->addWidget(scrollArea, 1);
    }

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_TPP__