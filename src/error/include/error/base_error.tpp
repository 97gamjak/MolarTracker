#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__
#define __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__

#include <format>
#include <mstd/type_traits/enum_traits.hpp>

#include "base_error.hpp"

/**
 * @brief Gets the private message of the error, this function is used to
 * retrieve the error message from the private member of the ErrorWrapper class,
 * providing a way to access the error message associated with the wrapped error
 * type.
 *
 * @tparam Error The error type to wrap, which must satisfy the IsError
 * concept.
 * @return const std::string& A const reference to the error message of the
 * wrapped error type.
 */
template <typename Error>
const std::string& ErrorWrapper<Error>::getMessage() const
{
    return error.getMessage();
}

/**
 * @brief Constructs an Error object.
 *
 * @tparam EnumType
 * @param type
 * @param message
 * @param subErrors
 * @param location (only used for debugging, not in release builds)
 */
template <ErrorTypeEnum EnumType>
Error<EnumType>::Error(
    EnumType                   type,
    std::optional<std::string> message,
    std::vector<Error>         subErrors
#ifndef NDEBUG
    ,
    std::source_location location
#endif
)
    : _type(type),
      _message(message.value_or(ErrorTypeMeta::toString(type))),
      _subErrors(std::move(subErrors))
#ifndef NDEBUG
      ,
      _location(location)
#endif
{
}

/**
 * @brief Gets the error type.
 *
 * @return EnumType
 */
template <ErrorTypeEnum EnumType>
EnumType Error<EnumType>::getType() const
{
    return _type;
}

/**
 * @brief Gets the string representation of the error type.
 *
 * @return std::string
 */
template <ErrorTypeEnum EnumType>
std::string Error<EnumType>::getTypeStr() const
{
    return ErrorTypeMeta::toString(_type);
}

/**
 * @brief Gets the sub-errors associated with the error, this function returns
 * a const reference to the vector of sub-errors, allowing for hierarchical
 * error representation and tracking of related errors.
 *
 * @return const std::vector<Error>& A const reference to the vector of
 * sub-errors associated with the error.
 */
template <ErrorTypeEnum EnumType>
const std::vector<Error<EnumType>>& Error<EnumType>::getSubErrors() const
{
    return _subErrors;
}

/**
 * @brief Gets the error message associated with the error, this function
 * returns a const reference to the error message, allowing for retrieval of
 * the error message for display or logging purposes.
 *
 * @return const std::string& A const reference to the error message
 * associated with the error.
 */
template <ErrorTypeEnum EnumType>
const std::string& Error<EnumType>::getMessage() const
{
    return _message;
}

/**
 * @brief compares two Error objects for equality, this function compares the
 * error type and error message of the two Error objects, returning true if they
 * are equal and false otherwise.
 *
 * @param other The other Error object to compare with.
 * @return true if the two Error objects are equal, false otherwise.
 */
template <ErrorTypeEnum EnumType>
bool Error<EnumType>::operator==(const Error& other) const
{
    return _type == other._type && _message == other._message;
}

/**
 * @brief Converts the error to a new error type, preserving the error message
 * and sub-errors. This function creates a new Error object of the specified
 * newType, with the same error message and sub-errors as the original error.
 *
 * @tparam EnumType
 * @param newType The new error type to convert to.
 * @param newMessage An optional new error message to use for the converted
 * error. If not provided, the original error message will be used.
 * @param addSubError A boolean flag indicating whether to add the original
 * error as a sub-error to the new error. If true, the original error will be
 * added as a sub-error to the new error.
 * @return Error<EnumType> A new Error object of the specified newType, with
 * the same error message and sub-errors as the original error.
 */
template <ErrorTypeEnum EnumType>
Error<EnumType> Error<EnumType>::convert(
    const EnumType&                   newType,
    const std::optional<std::string>& newMessage,
    bool                              addSubError
) const
{
    std::vector<Error<EnumType>> subErrors;
    bool                         containsSubError = false;

    const auto newSubError = Error<EnumType>(
        newType,
        getMessage(),
        {}
#ifndef NDEBUG
        ,
        _location
#endif
    );

    for (const auto& subError : _subErrors)
    {
        const auto error =
            subError.convert(newType, subError.getMessage(), false);
        subErrors.push_back(error);
        if (error == newSubError)
            containsSubError = true;
    }

    if (addSubError && newMessage && !containsSubError)
    {
        subErrors.push_back(newSubError);
    }

    return Error<EnumType>(
        newType,
        newMessage.value_or(getMessage()),
        std::move(subErrors)
#ifndef NDEBUG
            ,
        _location
#endif
    );
}

/**
 * @brief Converts the error to a new error type, preserving the error message
 * and sub-errors. This function creates a new Error object of the specified
 * newType, with the same error message and sub-errors as the original error.
 *
 * @tparam EnumType
 * @param newMessage An optional new error message to use for the converted
 * error. If not provided, the original error message will be used.
 *
 * @return Error<EnumType> A new Error object of the specified newType, with
 * the same error message and sub-errors as the original error.
 */
template <ErrorTypeEnum EnumType>
Error<EnumType> Error<EnumType>::convert(
    const std::optional<std::string>& newMessage
) const
{
    return convert(getType(), newMessage);
}

/**
 * @brief Converts the error to a string representation.
 *
 * @return std::string
 */
template <ErrorTypeEnum EnumType>
std::string Error<EnumType>::toString() const
{
    auto msg = std::format(
        "[{}] {}",
        mstd::enum_meta_t<EnumType>::toString(getType()),
        _message
    );

#ifndef NDEBUG
    msg += std::format(
        " (at {}:{} in function {})",
        _location.file_name(),
        _location.line(),
        _location.function_name()
    );
#endif

    for (const auto& subError : _subErrors)
        msg += "\n\t" + subError.toString();

    return msg;
}

/**
 * @brief Constructs an Result object with an error.
 *
 * @tparam T
 * @tparam E
 * @param error
 */
template <typename T, IsError E>
Result<T, E>::Result(const E& error)
    : std::expected<T, E>(std::unexpected(error))
{
}

#endif   // __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__
