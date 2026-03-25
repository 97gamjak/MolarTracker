#ifndef __UI__COMMANDS__COMMAND_ERROR_HPP__
#define __UI__COMMANDS__COMMAND_ERROR_HPP__

#include <memory>
#include <mstd/enum.hpp>
#include <string>

#define COMMAND_ERROR_TYPE_LIST(X) \
    X(InvalidCommand)              \
    X(NothingToRedo)               \
    X(NothingToUndo)               \
    X(UndoNotImplemented)          \
    X(RedoNotImplemented)          \
    X(UndoNotSupported)            \
    X(RedoNotSupported)

MSTD_ENUM(CommandErrorType, std::uint8_t, COMMAND_ERROR_TYPE_LIST);

namespace cmd
{
    /**
     * @brief Interface for command errors
     *
     */
    class ICommandError
    {
       public:
        virtual ~ICommandError() = default;

       public:
        /**
         * @brief Get the Message object
         *
         * @return std::string
         */
        [[nodiscard]] virtual std::string getMessage() const = 0;

        /**
         * @brief Get the Code Str object
         *
         * @return std::string
         */
        [[nodiscard]] virtual std::string getCodeStr() const = 0;
    };

    using CommandErrorPtr = std::shared_ptr<ICommandError>;

    /**
     * @brief Concrete implementation of ICommandError
     *
     */
    class CommandError : public ICommandError
    {
       private:
        /// The type of the command error
        CommandErrorType _type;

       public:
        explicit CommandError(CommandErrorType type);

        [[nodiscard]] std::string getMessage() const override;
        [[nodiscard]] std::string getCodeStr() const override;

        [[nodiscard]] static CommandErrorPtr makeNothingToUndoErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeNothingToRedoErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeInvalidCommandErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeUndoNotImplementedErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeRedoNotImplementedErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeUndoNotSupportedErrorPtr();
        [[nodiscard]] static CommandErrorPtr makeRedoNotSupportedErrorPtr();
    };

}   // namespace cmd

#endif   // __UI__COMMANDS__COMMAND_ERROR_HPP__
