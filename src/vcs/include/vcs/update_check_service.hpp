#ifndef __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__
#define __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__

#include <QFutureWatcher>
#include <QObject>
#include <QTimer>
#include <expected>
#include <optional>

#include "http/http_error.hpp"
#include "utils/version.hpp"
#include "vcs/github_client.hpp"

namespace vcs
{

    /**
     * @brief Periodically checks GitHub for a newer MolarTracker release.
     *
     * Fires an immediate check on start(), then repeats every 24 h. Emits
     * updateAvailable() at most once per distinct version so the same release
     * does not trigger multiple notifications within a single session.
     *
     */
    class UpdateCheckService : public QObject
    {
        Q_OBJECT

       public:
        explicit UpdateCheckService(QObject* parent = nullptr);

        void start();

       signals:
        void updateAvailable(utils::SemVer latestVersion);

       private slots:
        void onTimerTick();
        void onFetchFinished();

       private:
        GitHubClient                                                  _client;
        QTimer                                                        _timer;
        QFutureWatcher<std::expected<utils::SemVer, http::HttpError>> _watcher;
        std::optional<utils::SemVer> _lastNotifiedVersion;

        static constexpr int k_intervalMs = 24 * 60 * 60 * 1000;
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__
