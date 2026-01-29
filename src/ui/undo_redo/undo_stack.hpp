#ifndef __UI__UNDO_REDO__UNDO_STACK_HPP__
#define __UI__UNDO_REDO__UNDO_STACK_HPP__

#include <QObject>
#include <memory>
#include <vector>

#include "command.hpp"

namespace ui
{

    class UndoStack : public QObject
    {
        Q_OBJECT

       private:
        std::vector<std::unique_ptr<ICommand>> _commands;
        std::size_t                            _cursor = 0;

       signals:
        void changed();

       public:
        template <typename Cmd, typename... Args>
        bool make_and_do(Args&&... args)
        {
            auto cmd = std::make_unique<Cmd>(std::forward<Args>(args)...);
            return do_(std::move(cmd));
        }

        bool do_(std::unique_ptr<ICommand> command);

        bool canUndo() const;
        bool canRedo() const;

        void undo();
        bool redo();

        std::string undoLabel() const;
        std::string redoLabel() const;
    };

}   // namespace ui

#endif   // __UI__UNDO_REDO__UNDO_STACK_HPP__
