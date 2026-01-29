#ifndef __UI__UNDO_REDO__UNDO_STACK_HPP__
#define __UI__UNDO_REDO__UNDO_STACK_HPP__

#include <QObject>
#include <memory>
#include <utility>
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
        bool makeAndDo(Args&&... args)
        {
            auto cmd = std::make_unique<Cmd>(std::forward<Args>(args)...);
            return _do(std::move(cmd));
        }

        bool canUndo() const;
        bool canRedo() const;

        void undo();
        bool redo();

        std::string undoLabel() const;
        std::string redoLabel() const;

       private:
        bool _do(std::unique_ptr<ICommand> command);
    };

}   // namespace ui

#endif   // __UI__UNDO_REDO__UNDO_STACK_HPP__
