#ifndef __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__

#include <mstd/enum.hpp>

#include "error/base_error.hpp"

#define PARAM_ERROR_TYPE_LIST(X) \
    X(InvalidParamValue)         \
    X(ParamNotFound)             \
    GENERIC_ERRORS(X)

MSTD_ENUM(ParamErrorType, std::uint8_t, PARAM_ERROR_TYPE_LIST);

using ParamError = Error<ParamErrorType>;

template <typename T>
using ParamResult = Result<T, ParamError>;

#endif   // __ERROR__INCLUDE__ERROR__PARAM_ERROR_HPP__