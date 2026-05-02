#ifndef __HTTP__INCLUDE__HTTP__HTTP_ERROR_HPP__
#define __HTTP__INCLUDE__HTTP__HTTP_ERROR_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

namespace http
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define HTTP_ERROR_ENUM(X) \
    X(CurlInit)            \
    X(CurlPerform)         \
    X(BadStatus)           \
    X(ParseError)

    MSTD_ENUM(HttpErrorKind, std::uint8_t, HTTP_ERROR_ENUM);

    /**
     * @brief Represents an HTTP error
     *
     */
    struct HttpError
    {
        /// The kind of error
        HttpErrorKind kind;

        /// The HTTP status code, populated for BadStatus only
        int statusCode = 0;

        /// The error message from curl_easy_strerror or description
        std::string message;
    };

}   // namespace http

#endif   // __HTTP__INCLUDE__HTTP__HTTP_ERROR_HPP__