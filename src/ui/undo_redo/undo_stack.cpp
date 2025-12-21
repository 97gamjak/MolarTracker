#include "undo_stack.hpp"

void UndoStack::pushAndRedo(std::unique_ptr<ICommand> command)
{
    // If we are not at the end of the stack, remove all commands after the
    // cursor
    if (_cursor < _commands.size())
        _commands.erase(_commands.begin() + _cursor, _commands.end());

    command->redo();
    _commands.push_back(std::move(command));
    _cursor = _commands.size();
}

bool UndoStack::canUndo() const noexcept { return _cursor > 0; }

bool UndoStack::canRedo() const noexcept { return _cursor < _commands.size(); }

void UndoStack::undo()
{
    if (!canUndo())
        return;

    --_cursor;
    _commands[_cursor]->undo();
}

void UndoStack::redo()
{
    if (!canRedo())
        return;

    _commands[_cursor]->redo();
    ++_cursor;
}

std::string UndoStack::undoLabel() const
{
    if (!canUndo())
        return "";

    return _commands[_cursor - 1]->label();
}

std::string UndoStack::redoLabel() const
{
    if (!canRedo())
        return "";

    return _commands[_cursor]->label();
}