#include "http/http_client.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <fstream>

#include "error/http_error.hpp"
#include "http/curl.hpp"

namespace http
{
    namespace
    {
        /// Context passed to _fileWriteCallback via CURLOPT_WRITEDATA
        struct FileWriteContext
        {
            std::ofstream* file;
        };

        /// Context passed to _progressCallback via CURLOPT_XFERINFODATA
        struct ProgressContext
        {
            const DownloadProgressCallback* callback;
            const std::atomic<bool>*        cancelFlag;
        };

        /**
         * @brief libcurl write callback that appends received bytes to a
         * file stream.
         *
         * @param ptr Pointer to the data received
         * @param size Size of each data element
         * @param nMembers Number of data elements
         * @param userData Pointer to a FileWriteContext
         * @return size_t Number of bytes handled
         */
        size_t _fileWriteCallback(
            char*  ptr,
            size_t size,
            size_t nMembers,
            void*  userData
        )
        {
            auto*      context   = static_cast<FileWriteContext*>(userData);
            const auto byteCount = size * nMembers;
            context->file->write(ptr, static_cast<std::streamsize>(byteCount));
            return context->file->good() ? byteCount : 0;
        }

        /**
         * @brief libcurl progress callback used to report download progress
         * and support cooperative cancellation.
         *
         * @param userData Pointer to a ProgressContext
         * @param totalToDownload Total bytes to download (0 if unknown)
         * @param nowDownloaded Bytes downloaded so far
         * @return int Non-zero aborts the transfer
         */
        int _progressCallback(
            void*      userData,
            curl_off_t totalToDownload,
            curl_off_t nowDownloaded,
            curl_off_t /*totalToUpload*/,
            curl_off_t /*nowUploaded*/
        )
        {
            const auto* context = static_cast<const ProgressContext*>(userData);

            if (context->cancelFlag != nullptr && context->cancelFlag->load())
                return 1;

            if (*context->callback)
                (*context->callback)(nowDownloaded, totalToDownload);

            return 0;
        }
    }   // namespace

    /**
     * @brief Send a GET request
     *
     * @param request The HTTP request to send
     * @return The HTTP response or an error
     */
    HttpResult<HttpResponse> HttpClient::get(const HttpRequest& request)
    {
        CURL* curl = curl_easy_init();
        if (curl == nullptr)
        {
            return HttpError{
                HttpErrorType::CurlInit,
                "curl_easy_init returned null"
            };
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
            return HttpError{
                HttpErrorType::CurlPerform,
                curl_easy_strerror(res),
                std::move(responseHeaders),
            };
        }

        std::int64_t statusCode{};
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

        constexpr std::int64_t HTTP_STATUS_CODE_BAD_REQUEST = 400;
        if (statusCode >= HTTP_STATUS_CODE_BAD_REQUEST)
        {
            return HttpError{
                HttpErrorType::BadStatus,
                "HTTP error " + std::to_string(statusCode),
                responseHeaders,
                static_cast<int>(statusCode),
            };
        }

        return HttpResponse{
            .statusCode = statusCode,
            .body       = std::move(responseBody),
            .headers    = responseHeaders,
        };
    }

    /**
     * @brief Stream a GET response directly to a file on disk.
     *
     * @param request The HTTP request to send
     * @param destinationPath Where to write the downloaded file
     * @param progressCallback Optional progress reporting callback
     * @param cancelFlag Optional flag to cooperatively cancel the download
     * @return HttpResult<void>
     */
    HttpResult<void> HttpClient::downloadToFile(
        const HttpRequest&              request,
        const std::filesystem::path&    destinationPath,
        const DownloadProgressCallback& progressCallback,
        const std::atomic<bool>*        cancelFlag
    )
    {
        CURL* curl = curl_easy_init();
        if (curl == nullptr)
        {
            return HttpError{
                HttpErrorType::CurlInit,
                "curl_easy_init returned null"
            };
        }

        CurlGuard guard{curl};

        std::ofstream file(destinationPath, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
        {
            return HttpError{
                HttpErrorType::CurlInit,
                "failed to open destination file for writing"
            };
        }

        FileWriteContext writeContext{.file = &file};
        ProgressContext  progressContext{
             .callback   = &progressCallback,
             .cancelFlag = cancelFlag,
        };

        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _fileWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeContext);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Downloads can be large and slow; abort on a stall rather than a
        // fixed wall-clock timeout.
        constexpr long lowSpeedBytesPerSec = 1024;
        constexpr long lowSpeedTimeSeconds = 30;
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, lowSpeedBytesPerSec);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, lowSpeedTimeSeconds);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, _progressCallback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressContext);

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
        file.close();

        if (res != CURLE_OK)
        {
            std::error_code removeError;
            std::filesystem::remove(destinationPath, removeError);

            return HttpError{
                HttpErrorType::CurlPerform,
                curl_easy_strerror(res)
            };
        }

        std::int64_t statusCode{};
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

        constexpr std::int64_t HTTP_STATUS_CODE_BAD_REQUEST = 400;
        if (statusCode >= HTTP_STATUS_CODE_BAD_REQUEST)
        {
            std::error_code removeError;
            std::filesystem::remove(destinationPath, removeError);

            return HttpError{
                HttpErrorType::BadStatus,
                "HTTP error " + std::to_string(statusCode),
                {},
                static_cast<int>(statusCode),
            };
        }

        return {};
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