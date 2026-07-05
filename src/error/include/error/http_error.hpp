#ifndef __ERROR__INCLUDE__ERROR__HTTP_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__HTTP_ERROR_HPP__

#include <map>

#include "base_error.hpp"

#define HTTP_ERROR_TYPE_LIST(X) \
    X(CurlInit)                 \
    X(CurlPerform)              \
    X(BadStatus)                \
    X(ParseError)               \
    X(AuthError)

MSTD_ENUM(HttpErrorType, std::uint8_t, HTTP_ERROR_TYPE_LIST);

/**
 * @brief Represents an HTTP error
 *
 */
class HttpError : public Error<HttpErrorType>
{
   private:
    /// The HTTP status code, populated for BadStatus only
    int _statusCode;

    /// The response headers from the HTTP request
    std::map<std::string, std::string> _responseHeaders;

   public:
    HttpError(
        HttpErrorType                      type,
        std::string                        message,
        int                                statusCode,
        std::map<std::string, std::string> responseHeaders
    );
    HttpError(HttpErrorType type, std::string message);
    ~HttpError() override = default;

    [[nodiscard]]
    std::string toString() const override;

    [[nodiscard]]
    const std::map<std::string, std::string>& getResponseHeaders() const;
};

template <typename T>
using HttpResult = Result<T, HttpError>;

#endif   // __ERROR__INCLUDE__ERROR__HTTP_ERROR_HPP__