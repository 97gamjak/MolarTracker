#ifndef __UI__UNDO_REDO__UNDO_STACK_HPP__
#define __UI__UNDO_REDO__UNDO_STACK_HPP__

#include <memory>
#include <vector>

#include "command.hpp"

namespace ui
{

    class UndoStack
    {
       private:
        std::vector<std::unique_ptr<ICommand>> _commands;
        std::size_t                            _cursor = 0;

       public:
        void pushAndRedo(std::unique_ptr<ICommand> command);

        bool canUndo() const noexcept;
        bool canRedo() const noexcept;

        void undo();
        void redo();

        std::string undoLabel() const;
        std::string redoLabel() const;
    };

}   // namespace ui

#endif   // __UI__UNDO_REDO__UNDO_STACK_HPP__
