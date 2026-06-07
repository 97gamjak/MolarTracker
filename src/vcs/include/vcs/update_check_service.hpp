#ifndef __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__
#define __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__

#include <QFutureWatcher>
#include <QObject>
#include <QTimer>
#include <expected>
#include <optional>

#include "http/http_error.hpp"
#include "utils/version.hpp"

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

       private:
        /// The timer for scheduling periodic checks
        QTimer _timer;
        /// The watcher for the async GitHub fetch operation
        QFutureWatcher<std::expected<utils::SemVer, http::HttpError>> _watcher;
        /// The last version that was notified to the user, used for
        /// deduplication of updateAvailable() signals within the same session
        std::optional<utils::SemVer> _lastNotifiedVersion;

        // TODO(97gamjak): make this interval configurable and also add a way to
        // trigger manual checks from the UI
        /// The interval for periodic update checks in milliseconds (24 hours)
        static constexpr int _intervalMs = 24 * 60 * 60 * 1000;

       public:
        explicit UpdateCheckService(QObject* parent = nullptr);

        void start();

       signals:
        /**
         * @brief Signal emitted when a new MolarTracker release is available on
         * GitHub.
         *
         * @param latestVersion
         */
        void updateAvailable(utils::SemVer latestVersion);

       private slots:
        void onTimerTick();
        void onFetchFinished();
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__UPDATE_CHECK_SERVICE_HPP__
