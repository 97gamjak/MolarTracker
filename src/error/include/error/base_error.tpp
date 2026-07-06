#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__
#define __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__

#include <format>
#include <mstd/type_traits/enum_traits.hpp>

#include "base_error.hpp"

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
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
Error<EnumType>::Error(
    EnumType                   type,
    std::optional<std::string> message,
    std::vector<Error>         subErrors
)
    : _type(type),
      _message(message.value_or(ErrorTypeMeta::toString(type))),
      _subErrors(std::move(subErrors))
{
}

/**
 * @brief Gets the error type.
 *
 * @return EnumType
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
EnumType Error<EnumType>::getType() const
{
    return _type;
}

/**
 * @brief Gets the string representation of the error type.
 *
 * @return std::string
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
std::string Error<EnumType>::getTypeStr() const
{
    return ErrorTypeMeta::toString(_type);
}

template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
const std::vector<Error<EnumType>>& Error<EnumType>::getSubErrors() const
{
    return _subErrors;
}

template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
const std::string& Error<EnumType>::getMessage() const
{
    return _message;
}

template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
Error<EnumType> Error<EnumType>::convert(
    const EnumType&                   newType,
    const std::optional<std::string>& newMessage
) const
{
    std::vector<Error<EnumType>> subErrors;
    for (const auto& subError : _subErrors)
    {
        subErrors.push_back(subError.convert(newType, newMessage));
    }
    return Error<EnumType>(
        newType,
        newMessage.value_or(ErrorTypeMeta::toString(newType)),
        std::move(subErrors)
    );
}

/**
 * @brief Converts the error to a string representation.
 *
 * @return std::string
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
std::string Error<EnumType>::toString() const
{
    auto msg = std::format(
        "[{}] {}",
        mstd::enum_meta_t<EnumType>::toString(getType()),
        _message
    );

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