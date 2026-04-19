#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_HPP__

#include <QFormLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

#include "connections/connection.hpp"
#include "settings/params/param_utils.hpp"

namespace ui
{

    /**
     * @brief Shows all params for one ParamContainer section.
     *        Built entirely at compile time via _forEachParam — no runtime
     *        type erasure needed.
     *
     * @tparam TSection  Must satisfy IsParamContainer
     */
    template <settings::IsParamContainer TSection>
    class SettingsSection : public QWidget
    {
       private:
        std::vector<Connection> _connections;

       public:
        explicit SettingsSection(TSection& section);

        [[nodiscard]] std::vector<Connection>& getConnections();

        void _build(TSection& section);
    };

}   // namespace ui

#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_TPP__
#include "settings_section.tpp"
#endif

#endif   // __UI__INCLUDE__UI__SETTINGS__SETTINGS_SECTION_HPP__