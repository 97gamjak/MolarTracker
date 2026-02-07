#include <curl/curl.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "version.hpp"

namespace utils
{
    struct Result
    {
        bool        update_available = false;
        std::string current_tag;   // input
        std::string latest_tag;    // semver-highest found
        std::string error;         // non-empty on failure
    };

    inline std::size_t curl_write_cb(
        char*       ptr,
        std::size_t size,
        std::size_t nmemb,
        void*       userdata
    )
    {
        const std::size_t bytes = size * nmemb;
        auto*             out   = static_cast<std::string*>(userdata);
        out->append(ptr, bytes);
        return bytes;
    }

    inline std::optional<std::string> http_get(
        std::string_view url,
        std::string&     err_out
    )
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            err_out = "curl_easy_init failed";
            return std::nullopt;
        }

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, std::string(url).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curl_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // GitHub requires a User-Agent header
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "User-Agent: MolarTracker");
        headers =
            curl_slist_append(headers, "Accept: application/vnd.github+json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Reasonable timeouts
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 5000L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 8000L);

        // TLS verification ON (default). Explicitly keep it on.
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        const CURLcode rc = curl_easy_perform(curl);

        long status = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (rc != CURLE_OK)
        {
            err_out = std::string("curl_easy_perform failed: ") +
                      curl_easy_strerror(rc);
            return std::nullopt;
        }
        if (status < 200 || status >= 300)
        {
            err_out = "HTTP error " + std::to_string(status);
            return std::nullopt;
        }

        return response;
    }

    // Call once early in your program (or guard with std::call_once)
    inline void ensure_curl_global_init()
    {
        static bool initialized = false;
        if (!initialized)
        {
            const auto rc = curl_global_init(CURL_GLOBAL_DEFAULT);
            if (rc != 0)
                throw std::runtime_error("curl_global_init failed");
            initialized = true;
        }
    }

    inline Result check_github_tags_semver_latest(
        std::string_view owner,
        std::string_view repo,
        std::string_view current_tag
    )
    {
        Result r;
        r.current_tag = std::string(current_tag);

        SemVer current_sv;

        try
        {
            current_sv = SemVer{current_tag};
        }
        catch (const std::exception& e)
        {
            r.error = "Current tag is not semver: " + r.current_tag + " (" +
                      e.what() + ")";
            return r;
        }

        // Page 1, 100 tags. If you have more and need full coverage, add
        // pagination.
        const std::string url = "https://api.github.com/repos/" +
                                std::string(owner) + "/" + std::string(repo) +
                                "/tags?per_page=100&page=1";

        std::string err;
        const auto  body = http_get(url, err);
        if (!body)
        {
            r.error = err;
            return r;
        }

        nlohmann::json j;
        try
        {
            j = nlohmann::json::parse(*body);
        }
        catch (const std::exception& e)
        {
            r.error = std::string("JSON parse failed: ") + e.what();
            return r;
        }

        if (!j.is_array())
        {
            r.error = "Unexpected JSON (expected array)";
            return r;
        }

        SemVer      best      = current_sv;
        std::string best_tag  = r.current_tag;
        bool        found_any = false;

        for (const auto& item : j)
        {
            if (!item.is_object())
                continue;
            auto it = item.find("name");
            if (it == item.end() || !it->is_string())
                continue;

            const std::string tag = it->get<std::string>();
            SemVer            sv;
            try
            {
                sv = SemVer{tag};
            }
            catch (...)
            {
                continue;
            }

            found_any = true;
            if (sv > best)
            {
                best     = sv;
                best_tag = tag;
            }
        }

        if (!found_any)
        {
            r.error = "No semver tags found on page 1";
            return r;
        }

        r.latest_tag       = best_tag;
        r.update_available = (best > current_sv);
        return r;
    }
}   // namespace utils
