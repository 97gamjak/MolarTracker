#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__
#define __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__

#include <format>
#include <mstd/type_traits/enum_traits.hpp>

#include "config/error.hpp"

/**
 * @brief Constructs an Error object.
 *
 * @tparam EnumType
 * @param type
 * @param message
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
Error<EnumType>::Error(EnumType type, std::optional<std::string> message)
    : _type(type), _message(message.value_or(ErrorTypeMeta::toString(type)))
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
 * @brief Gets the error message.
 *
 * @return const std::string&
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
const std::string& Error<EnumType>::getMessage() const
{
    return _message;
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
    return std::format(
        "[{}] {}",
        mstd::enum_meta_t<EnumType>::toString(getType()),
        getMessage()
    );
}

/**
 * @brief Converts the error to a different error type.
 *
 * @tparam NewEnumType
 * @param error
 * @param newType
 * @return Error<NewEnumType>
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
template <typename NewEnumType>
Error<NewEnumType> Error<EnumType>::toError(
    const Error& error,
    NewEnumType  newType
)
{
    auto newError = Error<NewEnumType>{newType, error.getMessage()};

    for (const auto& subError : error._subErrors)
        newError._subErrors.push_back(
            Error<NewEnumType>::toError(subError, newType)
        );

    return newError;
}

template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
Error<EnumType> Error<EnumType>::fromError(
    const Error&               error,
    EnumType                   newType,
    std::optional<std::string> newMessage
)
{
    auto newError = Error<EnumType>{newType, newMessage};

    for (const auto& subError : error._subErrors)
        newError._subErrors.push_back(subError);

    return newError;
}
#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__