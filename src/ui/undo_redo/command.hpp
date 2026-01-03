#ifndef __UI__UNDO_REDO__COMMAND_HPP__
#define __UI__UNDO_REDO__COMMAND_HPP__

#include <string>

namespace ui
{

    /**
     * @brief Interface for undoable/redoable commands
     *
     */
    class ICommand
    {
       public:
        virtual ~ICommand() = default;

        virtual void undo() = 0;
        virtual void redo() = 0;

        virtual std::string label() const = 0;
    };

}   // namespace ui

#endif   // __UI__UNDO_REDO__COMMAND_HPP__