#ifndef __UI__COMMANDS__UNDO_STACK_HPP__
#define __UI__COMMANDS__UNDO_STACK_HPP__

#include <QObject>
#include <memory>
#include <vector>

#include "command_error.hpp"
#include "commands.hpp"

namespace ui
{

    /**
     * @brief Undo/redo stack for ICommand objects
     *
     */
    class UndoStack : public QObject
    {
        Q_OBJECT

       private:
        /// Vector of unique pointers to commands in the stack
        std::vector<Commands> _commands;

        /// Cursor pointing to the current position in the stack, this is used
        /// to determine which commands can be undone or redone, and to manage
        /// the state of the stack when new commands are added after undoing
        /// some commands
        std::size_t _cursor = 0;

       signals:
        /// Signal emitted when the undo stack changes
        void changed();

       public:
        void push(Commands commands);

        [[nodiscard]] bool canUndo() const;
        [[nodiscard]] bool canRedo() const;

        [[nodiscard]] std::expected<void, CommandErrorPtr> undo();
        [[nodiscard]] std::expected<void, CommandErrorPtr> redo();

        [[nodiscard]] std::string getUndoLabel() const;
        [[nodiscard]] std::string getRedoLabel() const;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__UNDO_STACK_HPP__
