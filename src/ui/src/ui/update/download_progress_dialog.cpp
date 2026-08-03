#include "ui/update/download_progress_dialog.hpp"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

#include "common/qt_helpers.hpp"
#include "config/constants/constants.hpp"
#include "http/http_client.hpp"
#include "http/http_request.hpp"
#include "vcs/asset_selector.hpp"
#include "vcs/github_client.hpp"

using common::makeQChild;

namespace ui
{
    namespace
    {
        /**
         * @brief Fetch the latest release assets, select the one matching
         * this platform, and download it to the update staging directory.
         *
         * @param cancelFlag Cooperative cancellation flag for the download
         * @param progressCallback Called with (downloaded, total) bytes
         * @return UpdateResult<std::filesystem::path> The downloaded
         * archive's path on success
         */
        UpdateResult<std::filesystem::path> _fetchAndDownloadUpdate(
            const std::atomic<bool>*              cancelFlag,
            const http::DownloadProgressCallback& progressCallback
        )
        {
            const auto assetsResult =
                vcs::GitHubClient::fetchLatestReleaseAssets();
            if (!assetsResult)
            {
                return FromError<HttpError, UpdateError>::apply(
                    assetsResult.error(),
                    UpdateErrorType::DownloadFailed,
                    "failed to fetch release information: " +
                        assetsResult.error().toString()
                );
            }

            const auto asset =
                vcs::selectAssetForCurrentPlatform(*assetsResult);
            if (!asset.has_value())
            {
                return UpdateError{
                    UpdateErrorType::NoCompatibleAsset,
                    "no compatible update package was found for this "
                    "platform in the latest release"
                };
            }

            const auto stagingRoot =
                Constants::getInstance().getUpdateStagingPath();
            std::error_code errorCode;
            std::filesystem::create_directories(stagingRoot, errorCode);

            const auto destination = stagingRoot / asset->name;

            const auto downloadResult = http::HttpClient::downloadToFile(
                http::HttpRequest{
                    .url     = asset->downloadUrl,
                    .headers = {{"User-Agent", "MolarTracker"}},
                },
                destination,
                progressCallback,
                cancelFlag
            );

            if (!downloadResult)
            {
                return FromError<HttpError, UpdateError>::apply(
                    downloadResult.error(),
                    UpdateErrorType::DownloadFailed,
                    downloadResult.error().toString()
                );
            }

            return destination;
        }
    }   // namespace

    /**
     * @brief Construct a DownloadProgressDialog and start the lookup +
     * download immediately.
     *
     * @param parent Optional parent widget
     */
    DownloadProgressDialog::DownloadProgressDialog(QWidget* parent)
        : Dialog{parent}
    {
        setWindowTitle("Downloading Update...");
        _buildUI();

        connect(
            &_watcher,
            &QFutureWatcher<UpdateResult<std::filesystem::path>>::finished,
            this,
            &DownloadProgressDialog::_onFinished
        );
        connect(
            this,
            &DownloadProgressDialog::progressReported,
            this,
            &DownloadProgressDialog::_onProgress
        );

        _watcher.setFuture(
            QtConcurrent::run(
                [this]() -> UpdateResult<std::filesystem::path>
                {
                    return _fetchAndDownloadUpdate(
                        &_cancelRequested,
                        [this](std::int64_t downloaded, std::int64_t total)
                        { emit progressReported(downloaded, total); }
                    );
                }
            )
        );
    }

    /**
     * @brief Build the UI layout for the download progress dialog.
     */
    void DownloadProgressDialog::_buildUI()
    {
        auto* layout = makeQChild<QVBoxLayout>(this);

        _statusLabel =
            makeQChild<QLabel>("Looking up the latest release...", this);
        layout->addWidget(_statusLabel);

        _progressBar = makeQChild<QProgressBar>(this);
        _progressBar->setRange(0, 100);
        _progressBar->setValue(0);
        layout->addWidget(_progressBar);

        _cancelButton = makeQChild<QPushButton>("Cancel", this);
        layout->addWidget(_cancelButton);

        connect(
            _cancelButton,
            &QPushButton::clicked,
            this,
            &DownloadProgressDialog::_onCancelClicked
        );
    }

    /**
     * @brief Update the progress bar and status label.
     *
     * @param downloaded Bytes downloaded so far
     * @param total Total bytes to download (0 if unknown)
     */
    void DownloadProgressDialog::_onProgress(qint64 downloaded, qint64 total)
    {
        if (total <= 0)
            return;

        constexpr double percentScale = 100.0;
        const auto       percent      = static_cast<int>(
            static_cast<double>(downloaded) / static_cast<double>(total) *
            percentScale
        );
        _progressBar->setValue(percent);

        constexpr double bytesPerMebibyte = 1024.0 * 1024.0;
        _statusLabel->setText(
            QString("Downloaded %1 MB of %2 MB")
                .arg(
                    static_cast<double>(downloaded) / bytesPerMebibyte,
                    0,
                    'f',
                    1
                )
                .arg(static_cast<double>(total) / bytesPerMebibyte, 0, 'f', 1)
        );
    }

    /**
     * @brief Handle the download finishing (successfully, with an error, or
     * cancelled) — records the result and closes the dialog.
     */
    void DownloadProgressDialog::_onFinished()
    {
        _result = _watcher.result();

        if (_result->has_value())
            accept();
        else
            reject();
    }

    /**
     * @brief Handle the Cancel button — requests cooperative cancellation of
     * the in-progress download.
     */
    void DownloadProgressDialog::_onCancelClicked()
    {
        _cancelRequested.store(true);
        _cancelButton->setEnabled(false);
        _statusLabel->setText("Cancelling...");
    }

    /**
     * @brief The outcome of the download.
     *
     * @return const std::optional<UpdateResult<std::filesystem::path>>&
     */
    const std::optional<UpdateResult<std::filesystem::path>>& DownloadProgressDialog::
        result() const
    {
        return _result;
    }

}   // namespace ui
