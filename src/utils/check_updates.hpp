#include <iostream>

#include "git.hpp"

namespace utils
{

    void check_for_updates()
    {
        ensure_curl_global_init();

        // from your embedded macro
        const auto current =
#if MOLARTRACKER_IS_TAG
            MOLARTRACKER_GIT_TAG;
#else
            "0.0.0";
#endif

        const auto res = check_github_tags_semver_latest(
            "97gamjak",
            "MolarTracker",
            current
        );

        if (!res.error.empty())
        {
            std::cerr << "Update check failed: " << res.error << std::endl;
            return;
        }

        if (res.update_available)
        {
            // show dialog / notify: res.latest_tag
        }
    }

}   // namespace utils