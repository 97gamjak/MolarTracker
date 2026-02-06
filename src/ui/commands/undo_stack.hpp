#ifndef __UI__COMMANDS__UNDO_STACK_HPP__
#define __UI__COMMANDS__UNDO_STACK_HPP__

#include <QObject>
#include <memory>
#include <utility>
#include <vector>

#include "command.hpp"
#include "command_error.hpp"

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
        std::vector<std::unique_ptr<ICommand>> _commands;
        std::size_t                            _cursor = 0;

       signals:
        void changed();

       public:
        /**
         * @brief Create and execute a command
         *
         * @tparam Cmd
         * @tparam Args
         * @param args
         * @return std::expected<void, CommandErrorPtr>
         */
        template <typename Cmd, typename... Args>
        std::expected<void, CommandErrorPtr> makeAndDo(Args&&... args)
        {
            auto cmd = std::make_unique<Cmd>(std::forward<Args>(args)...);
            return _do(std::move(cmd));
        }

        bool canUndo() const;
        bool canRedo() const;

        std::expected<void, CommandErrorPtr> undo();
        std::expected<void, CommandErrorPtr> redo();

        std::string getUndoLabel() const;
        std::string getRedoLabel() const;

       private:
        std::expected<void, CommandErrorPtr> _do(std::unique_ptr<ICommand> cmd);
    };

}   // namespace ui

#endif   // __UI__COMMANDS__UNDO_STACK_HPP__
