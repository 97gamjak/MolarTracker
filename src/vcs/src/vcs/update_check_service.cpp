#include "vcs/update_check_service.hpp"

#include <QtConcurrent/QtConcurrent>

#include "logging/log_macros.hpp"
#include "vcs/github_client.hpp"

REGISTER_LOG_CATEGORY("VCS.UpdateCheckService");

namespace vcs
{

    /**
     * @brief Construct a new UpdateCheckService.
     *
     * @param parent Qt parent object
     */
    UpdateCheckService::UpdateCheckService(QObject* parent) : QObject{parent}
    {
        connect(
            &_timer,
            &QTimer::timeout,
            this,
            &UpdateCheckService::onTimerTick
        );
        connect(
            &_watcher,
            &QFutureWatcher<
                std::expected<utils::SemVer, http::HttpError>>::finished,
            this,
            &UpdateCheckService::onFetchFinished
        );
    }

    /**
     * @brief Start the update check service.
     *
     * Fires an immediate check and then starts the 24-hour periodic timer.
     */
    void UpdateCheckService::start()
    {
        LOG_INFO("Update check service started");
        onTimerTick();
        _timer.start(_intervalMs);
    }

    /**
     * @brief Slot called on each timer tick (and once on start).
     *
     * Skips the check if a previous fetch is still in flight.
     */
    void UpdateCheckService::onTimerTick()
    {
        if (_watcher.isRunning())
        {
            LOG_DEBUG("Skipping update check — previous fetch still in flight");
            return;
        }

        _watcher.setFuture(
            QtConcurrent::run([]()
                              { return GitHubClient::fetchLatestVersion(); })
        );
    }

    /**
     * @brief Slot called when the async fetch completes.
     *
     * Compares the fetched version against the running version and emits
     * updateAvailable() if a newer release is found, deduplicating within the
     * same session.
     */
    void UpdateCheckService::onFetchFinished()
    {
        const auto result = _watcher.result();

        if (!result)
        {
            LOG_WARNING(
                "Update check failed: " + std::string{result.error().message}
            );
            return;
        }

        const utils::SemVer& latest = *result;

        if (latest <= utils::SemVer::current())
            return;

        if (_lastNotifiedVersion.has_value() && *_lastNotifiedVersion == latest)
            return;

        _lastNotifiedVersion = latest;
        emit updateAvailable(latest);
    }

}   // namespace vcs
