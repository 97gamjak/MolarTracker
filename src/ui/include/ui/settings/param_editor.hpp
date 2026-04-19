#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__
#define __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__

class QWidget;   // Forward declaration

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

}   // namespace ui

#ifndef __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_TPP__
#include "param_editor.tpp"
#endif

#endif   // __UI__INCLUDE__UI__SETTINGS__PARAM_EDITOR_HPP__