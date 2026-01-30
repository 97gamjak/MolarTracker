#include "undo_stack.hpp"

namespace ui
{

    bool UndoStack::_do(std::unique_ptr<ICommand> command)
    {
        if (!command)
            return false;

        // Execute first; only push on success.
        if (!command->redo())
            return false;

        // Drop redo history (everything after cursor).
        if (_cursor < _commands.size())
        {
            _commands.erase(
                _commands.begin() + static_cast<std::ptrdiff_t>(_cursor),
                _commands.end()
            );
        }

        _commands.push_back(std::move(command));
        _cursor = _commands.size();

        emit changed();
        return true;
    }

    bool UndoStack::canUndo() const { return _cursor > 0; }

    bool UndoStack::canRedo() const { return _cursor < _commands.size(); }

    void UndoStack::undo()
    {
        if (!canUndo())
            return;

        --_cursor;
        _commands[_cursor]->undo();

        emit changed();
    }

    bool UndoStack::redo()
    {
        if (!canRedo())
            return false;

        // Policy: if redo fails, keep cursor unchanged and do not advance.
        if (!_commands[_cursor]->redo())
            return false;

        ++_cursor;

        emit changed();

        return true;
    }

    std::string UndoStack::undoLabel() const
    {
        if (!canUndo())
            return "";

        return _commands[_cursor - 1]->getLabel();
    }

    std::string UndoStack::redoLabel() const
    {
        if (!canRedo())
            return "";

        return _commands[_cursor]->getLabel();
    }

}   // namespace ui