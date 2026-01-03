#ifndef __UI__UNDO_REDO__TOGGLE_FLAG_COMMAND_HPP__
#define __UI__UNDO_REDO__TOGGLE_FLAG_COMMAND_HPP__

#include "command.hpp"

namespace ui
{

    class ToggleFlagCommand : public ICommand
    {
       private:
        bool&       _flag;
        std::string _label;

       public:
        ToggleFlagCommand(bool& flag, const std::string& label)
            : _flag{flag}, _label{label}
        {
        }

        void undo() override { _flag = !_flag; }
        void redo() override { _flag = !_flag; }

        std::string label() const override { return _label; }
    };

}   // namespace ui

#endif   // __UI__UNDO_REDO__TOGGLE_FLAG_COMMAND_HPP__