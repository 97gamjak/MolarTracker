#ifndef __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__

#include <mstd/enum.hpp>

#include "base_error.hpp"

#define PARAM_ERROR_LIST(X) \
    X(InvalidValue)         \
    X(OutOfRange)           \
    X(MissingValue)         \
    X(InvalidType)          \
    X(UnknownError)         \
    X(DuplicateValue)

MSTD_ENUM(ParamErrorType, std::uint8_t, PARAM_ERROR_LIST);

using ParamError = Error<ParamErrorType>;

template <typename T>
using ParamResult = Result<T, ParamError>;

#define PARAM_ERROR_ENUM_NAME ParamErrorType

#endif   // __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__