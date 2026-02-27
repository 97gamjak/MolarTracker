#ifndef __ORM__CRUD__CRUD_ERROR_HPP__
#define __ORM__CRUD__CRUD_ERROR_HPP__

#include <cstdint>
#include <string>

#include "exceptions/base.hpp"
#include "mstd/enum.hpp"

namespace orm
{
    // NOLINTBEGIN
#define CRUD_ERROR_LIST(X) \
    X(NoRowsUpdated)       \
    X(NoPrimaryKey)        \
    X(MultipleRowsUpdated) \
    X(InsertFailed)        \
    X(NotFound)

    MSTD_ENUM(CrudErrorType, uint8_t, CRUD_ERROR_LIST);
    // NOLINTEND

    /**
     * @brief Exception class for CRUD operation errors
     *
     */
    class CrudException : public MolarTrackerException
    {
       public:
        explicit CrudException(const std::string& message);

        [[nodiscard]] const char* what() const noexcept override;
    };

    /**
     * @brief Error class for CRUD operation return values
     *
     */
    class CrudError
    {
       private:
        /// The type of the CRUD error
        CrudErrorType _type;

        /// The error message describing the CRUD error
        std::string _message;

       public:
        CrudError(CrudErrorType type, std::string message);

        [[nodiscard]] CrudErrorType      getType() const;
        [[nodiscard]] const std::string& getMessage() const;
    };

}   // namespace orm

#endif   // __ORM__CRUD__CRUD_ERROR_HPP__