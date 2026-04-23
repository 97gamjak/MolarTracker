#include "http/http_client.hpp"

#include <curl/curl.h>

#include "http/curl.hpp"

namespace http
{

    /**
     * @brief Send a GET request
     *
     * @param request The HTTP request to send
     * @return The HTTP response or an error
     */
    std::expected<HttpResponse, HttpError> HttpClient::get(
        const HttpRequest& request
    )
    {
        CURL* curl = curl_easy_init();
        if (curl == nullptr)
        {
            return std::unexpected(
                HttpError{
                    .kind    = HttpErrorKind::CurlInit,
                    .message = "curl_easy_init returned null",
                }
            );
        }

        // RAII cleanup
        CurlGuard guard{curl};

        std::string responseBody;

        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, request.timeoutSeconds);

        // Build headers
        curl_slist* headerList = nullptr;
        HeaderGuard headerGuard{&headerList};

        for (const auto& [key, value] : request.headers)
        {
            std::string header  = key + ": ";
            header             += value;
            headerList          = curl_slist_append(headerList, header.c_str());
        }

        if (headerList != nullptr)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);

        const CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            return std::unexpected(
                HttpError{
                    .kind    = HttpErrorKind::CurlPerform,
                    .message = curl_easy_strerror(res),
                }
            );
        }

        std::int64_t statusCode{};
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

        constexpr std::int64_t HTTP_STATUS_CODE_BAD_REQUEST = 400;
        if (statusCode >= HTTP_STATUS_CODE_BAD_REQUEST)
        {
            return std::unexpected(
                HttpError{
                    .kind       = HttpErrorKind::BadStatus,
                    .statusCode = static_cast<int>(statusCode),
                    .message    = "HTTP error " + std::to_string(statusCode),
                }
            );
        }

        return HttpResponse{
            .statusCode = statusCode,
            .body       = std::move(responseBody),
        };
    }

    /**
     * @brief Callback function for handling data received from the HTTP
     * response
     *
     * @param ptr Pointer to the data received
     * @param size Size of each data element
     * @param nMembers Number of data elements
     * @param data String to append the received data
     * @return size_t Number of bytes handled
     */
    size_t HttpClient::_writeCallback(
        char*        ptr,
        size_t       size,
        size_t       nMembers,
        std::string* data
    )
    {
        data->append(ptr, size * nMembers);
        return size * nMembers;
    }

}   // namespace http