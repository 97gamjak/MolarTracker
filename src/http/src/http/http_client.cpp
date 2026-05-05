#include "http/http_client.hpp"

#include <curl/curl.h>

#include <algorithm>

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
                    .kind            = HttpErrorKind::CurlInit,
                    .message         = "curl_easy_init returned null",
                    .responseHeaders = {},
                }
            );
        }

        // RAII cleanup
        CurlGuard guard{curl};

        std::string                        responseBody;
        std::map<std::string, std::string> responseHeaders;

        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, request.timeoutSeconds);

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);

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
                    .kind            = HttpErrorKind::CurlPerform,
                    .message         = curl_easy_strerror(res),
                    .responseHeaders = std::move(responseHeaders),
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
                    .responseHeaders = responseHeaders,
                }
            );
        }

        return HttpResponse{
            .statusCode = statusCode,
            .body       = std::move(responseBody),
            .headers    = responseHeaders,
        };
    }

    /**
     * @brief URL-encode a string
     *
     * @param value The string to encode
     * @return std::string The URL-encoded string
     */
    std::string HttpClient::urlEncode(const std::string& value)
    {
        CURL* curl = curl_easy_init();
        if (curl == nullptr)
            return value;   // fallback — better than crashing

        char* encoded = curl_easy_escape(
            curl,
            value.c_str(),
            static_cast<int>(value.size())
        );
        std::string result = encoded != nullptr ? encoded : value;

        curl_free(encoded);
        curl_easy_cleanup(curl);
        return result;
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

    /**
     * @brief Callback function for handling headers received from the HTTP
     * response
     *
     * @param ptr Pointer to the header data received
     * @param size Size of each data element
     * @param nmemb Number of data elements
     * @param headers Map to store the extracted headers
     * @return size_t Number of bytes handled
     */
    size_t HttpClient::headerCallback(
        char*                               ptr,
        size_t                              size,
        size_t                              nmemb,
        std::map<std::string, std::string>* headers
    )
    {
        std::string line(ptr, size * nmemb);

        // strip \r\n
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
            line.pop_back();

        const auto colon = line.find(':');
        if (colon == std::string::npos)
            return size * nmemb;   // status line or empty — skip

        std::string key   = line.substr(0, colon);
        std::string value = line.substr(colon + 2);   // skip ": "

        // Lowercase the key for case-insensitive lookup
        std::ranges::transform(key, key.begin(), ::tolower);
        (*headers)[std::move(key)] = std::move(value);

        return size * nmemb;
    }

}   // namespace http