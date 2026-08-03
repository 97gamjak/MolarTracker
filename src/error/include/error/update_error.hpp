#ifndef __ERROR__INCLUDE__ERROR__UPDATE_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__UPDATE_ERROR_HPP__

#include <mstd/enum.hpp>

#include "error/base_error.hpp"

#define UPDATE_ERROR_TYPE_LIST(X) \
    X(NoCompatibleAsset)          \
    X(DownloadFailed)             \
    X(Cancelled)                  \
    X(ExtractionFailed)           \
    X(StagingVerificationFailed)  \
    X(ApplyFailed)                \
    GENERIC_ERRORS(X)

MSTD_ENUM(UpdateErrorType, std::uint8_t, UPDATE_ERROR_TYPE_LIST);

using UpdateError = Error<UpdateErrorType>;

template <typename T>
using UpdateResult = Result<T, UpdateError>;

#endif   // __ERROR__INCLUDE__ERROR__UPDATE_ERROR_HPP__
