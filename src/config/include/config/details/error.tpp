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
Error<EnumType>::Error(EnumType type, std::string message)
    : _type(type), _message(std::move(message))
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
    return Error<NewEnumType>{newType, error.getMessage()};
}

#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__