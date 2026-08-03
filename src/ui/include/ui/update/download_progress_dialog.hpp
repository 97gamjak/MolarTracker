#ifndef __UI__INCLUDE__UI__UPDATE__DOWNLOAD_PROGRESS_DIALOG_HPP__
#define __UI__INCLUDE__UI__UPDATE__DOWNLOAD_PROGRESS_DIALOG_HPP__

#include <QFutureWatcher>
#include <atomic>
#include <filesystem>
#include <optional>

#include "error/update_error.hpp"
#include "ui/base/dialog.hpp"

class QLabel;
class QProgressBar;
class QPushButton;

namespace ui
{

    /**
     * @brief Dialog that looks up the release asset matching the current
     * platform, downloads it to disk, and shows progress, with a Cancel
     * button.
     *
     * The lookup + download starts as soon as the dialog is constructed. The
     * dialog closes itself (accept() on success, reject() on
     * failure/cancel) once finished; the caller should check result() after
     * exec() returns.
     */
    class DownloadProgressDialog : public Dialog
    {
        Q_OBJECT

       private:
        /// Progress bar showing download percentage
        QProgressBar* _progressBar = nullptr;
        /// Label showing status/bytes downloaded / total
        QLabel* _statusLabel = nullptr;
        /// Button to cancel the in-progress download
        QPushButton* _cancelButton = nullptr;

        /// Cooperative cancellation flag checked by the download's progress
        /// callback
        std::atomic<bool> _cancelRequested{false};
        /// Watches the background lookup+download future
        QFutureWatcher<UpdateResult<std::filesystem::path>> _watcher;
        /// The outcome of the download, populated once finished: the path to
        /// the downloaded archive on success
        std::optional<UpdateResult<std::filesystem::path>> _result;

        void _buildUI();

       private slots:
        void _onProgress(qint64 downloaded, qint64 total);
        void _onFinished();
        void _onCancelClicked();

       signals:
        /// Emitted from the download's worker thread to marshal progress
        /// updates onto the GUI thread
        void progressReported(qint64 downloaded, qint64 total);

       public:
        explicit DownloadProgressDialog(QWidget* parent);

        /**
         * @brief The outcome of the download: the path to the downloaded
         * archive on success.
         *
         * Only meaningful after exec() has returned.
         *
         * @return const std::optional<UpdateResult<std::filesystem::path>>&
         */
        [[nodiscard]]
        const std::optional<
            UpdateResult<std::filesystem::path>>& result() const;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__UPDATE__DOWNLOAD_PROGRESS_DIALOG_HPP__
