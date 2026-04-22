#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__
#define __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__

#include <cstdint>
#include <vector>

#include "connections/connection.hpp"
#include "settings/params/bool_param.hpp"
#include "settings/params/string_param.hpp"

class QWidget;       // Forward declaration
class QFormLayout;   // Forward declaration

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
    [[nodiscard]] QWidget* makeParamEditor(TParam& param);

    [[nodiscard]] QWidget* makeBoolEditor(settings::BoolParam& param);
    [[nodiscard]] QWidget* makeStringEditor(settings::StringParam& param);

    template <typename T>
    [[nodiscard]] QWidget* makeNumericEditor(T& param);

    template <typename T>
    [[nodiscard]] QWidget* makeNumericVecEditor(T& param);

    template <typename T>
    [[nodiscard]] QWidget* makeEnumEditor(T& param);

    void onDirtyStripe(void* userData, const bool& isDirty);

    enum class SectionMode : std::uint8_t
    {
        LeavesOnly,
        SubContainersOnly,
        All
    };

    template <typename TParam>
    void buildParamRows(
        TParam&                  param,
        QFormLayout*             layout,
        std::vector<Connection>& connections,
        SectionMode              mode
    );

}   // namespace ui

#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
#include "param_editor.tpp"
#endif

#endif   // __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__