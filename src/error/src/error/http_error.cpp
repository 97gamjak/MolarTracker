#include "error/http_error.hpp"

HttpError::HttpError(
    HttpErrorType                      type,
    std::string                        message,
    int                                statusCode,
    std::map<std::string, std::string> responseHeaders
)
    : Error<HttpErrorType>(type, message),
      _statusCode{statusCode},
      _responseHeaders{std::move(responseHeaders)}
{
}

HttpError::HttpError(HttpErrorType type, std::string message)
    : Error<HttpErrorType>(type, message), _statusCode{0}
{
}

const std::map<std::string, std::string>& HttpError::getResponseHeaders() const
{
    return _responseHeaders;
}

std::string HttpError::toString() const
{
    return std::format(
        "HttpError(type={}, message={}, statusCode={}, responseHeaders={})",
        getTypeStr(),
        getMessage(),
        _statusCode,
        [&]()
        {
            std::string headersStr;
            for (const auto& [key, value] : _responseHeaders)
            {
                headersStr += std::format("{}: {}, ", key, value);
            }
            return headersStr;
        }()
    );
}