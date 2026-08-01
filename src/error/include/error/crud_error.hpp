#ifndef __ERROR__INCLUDE__ERROR__CRUD_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__CRUD_ERROR_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

#include "base_error.hpp"

#define CRUDE_ERROR_TYPE_LIST(X) \
    X(NoRowsUpdated)             \
    X(NoPrimaryKey)              \
    X(MultipleRowsUpdated)       \
    X(InsertFailed)              \
    X(UpdateFailed)              \
    X(NotFound)                  \
    X(MultipleResults)           \
    X(ColumnAlreadyExists)       \
    X(ColumnDoesNotExist)        \
    X(DatabaseError)             \
    GENERIC_ERRORS(X)

#define DATABASE_ERROR_LIST(X)      \
    X(SqliteError)                  \
    X(BeginTransactionFailed)       \
    X(CommitTransactionFailed)      \
    X(RollbackTransactionFailed)    \
    X(ForeignKeyPragmaChangeFailed) \
    X(OpeningFailed)                \
    X(RestoreFromBackupFailed)      \
    GENERIC_ERRORS(X)

MSTD_ENUM(CrudErrorType, std::uint8_t, CRUDE_ERROR_TYPE_LIST);
MSTD_ENUM(DatabaseErrorType, std::uint8_t, DATABASE_ERROR_LIST);

using CrudError     = Error<CrudErrorType>;
using DatabaseError = Error<DatabaseErrorType>;

template <typename T>
using CrudResult = Result<T, CrudError>;

template <typename T>
using DatabaseResult = Result<T, DatabaseError>;

/**
 * @brief Converts an error of type DatabaseError to an error of type
 * YFinanceError, preserving the error message and sub-errors.
 */
template <>
struct FromError<DatabaseError, CrudError>
{
    /**
     * @brief Converts an error of type DatabaseError to an error of type
     * CrudError, preserving the error message and sub-errors.
     *
     * @param error The original DatabaseError to convert.
     * @param newMessage An optional new error message to use for the converted
     * error. If not provided, the original error message will be used.
     * @return CrudError A new CrudError object with the same message and
     * sub-errors as the original DatabaseError.
     */
    // NOLINTNEXTLINE(misc-no-recursion)
    static CrudError apply(
        const DatabaseError&              error,
        const std::optional<std::string>& newMessage = std::nullopt
    )
    {
        std::vector<CrudError> subErrors;
        for (const auto& subError : error.getSubErrors())
        {
            subErrors.push_back(
                FromError<DatabaseError, CrudError>::apply(subError)
            );
        }

        return CrudError(
            CrudErrorType::DatabaseError,
            newMessage.value_or(ErrorWrapper{error}.getMessage()),
            subErrors
        );
    }
};

#endif   // __ERROR__INCLUDE__ERROR__CRUD_ERROR_HPP__
