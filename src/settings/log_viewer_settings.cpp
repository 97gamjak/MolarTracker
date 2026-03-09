#include "log_viewer_settings.hpp"

#include "config/constants.hpp"
#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Construct a new LogViewerSettings::LogViewerSettings object
     *
     */
    LogViewerSettings::LogViewerSettings()
    {
        _reloadIntervalSec.setDefault(Schema::RELOAD_INTERVAL_SEC_DEFAULT);
        _reloadIntervalSec.setMinValue(Schema::RELOAD_INTERVAL_SEC_MIN);
        _reloadIntervalSec.setPrecision(Schema::RELOAD_INTERVAL_SEC_PRECISION);

        _autoReload.setDefault(Schema::AUTO_RELOAD_DEFAULT);

        _lineWrap.setDefault(Schema::LINE_WRAP_DEFAULT);

        _dialogSize.setDefaults(Schema::DIALOG_SIZE_DEFAULT);
        _dialogSize.setMinValues(Constants::getGlobalMinDialogSize());

        _maxBlockCount.setDefault(Schema::MAX_BLOCK_COUNT_DEFAULT);
        _maxBlockCount.setMinValue(Schema::MAX_BLOCK_COUNT_MIN);
    }

    /**
     * @brief Get the reload interval in seconds
     *
     * @return double
     */
    double LogViewerSettings::getReloadIntervalSec() const
    {
        return _reloadIntervalSec.get();
    }

    /**
     * @brief Get the reload interval in milliseconds
     *
     * @return int
     */
    int LogViewerSettings::getReloadIntervalMs() const
    {
        // ensure that the precision is 3 decimal places for correct
        // conversion to milliseconds
        assert(
            _reloadIntervalSec.getPrecision().has_value() &&
            _reloadIntervalSec.getPrecision().value() == 3
        );

        return static_cast<int>(
            getReloadIntervalSec() * Constants::getSecondsToMs()
        );
    }

    /**
     * @brief Get the auto-reload setting
     *
     * @return bool
     */
    bool LogViewerSettings::isAutoReloadEnabled() const
    {
        return _autoReload.get();
    }

    /**
     * @brief Get the line wrap setting
     *
     * @return bool
     */
    bool LogViewerSettings::isLineWrapEnabled() const
    {
        return _lineWrap.get();
    }

    /**
     * @brief Get the dialog size setting
     *
     * @return std::pair<int, int>
     */
    std::pair<int, int> LogViewerSettings::getDialogSize() const
    {
        return {_dialogSize.x(), _dialogSize.y()};
    }

    /**
     * @brief Get the maximum block count setting
     *
     * @return int
     */
    int LogViewerSettings::getMaxBlockCount() const
    {
        return _maxBlockCount.get();
    }

    /**
     * @brief Construct a new
     * LogViewerSettingsException::LogViewerSettingsException object
     *
     * @param message The exception message
     */
    LogViewerSettingsException::LogViewerSettingsException(
        const std::string& message
    )
        : MolarTrackerException(message)
    {
    }

    /**
     * @brief Get the exception message
     *
     * @return const char* The exception message
     */
    const char* LogViewerSettingsException::what() const noexcept
    {
        static const std::string prefix = "LogViewerSettingsException: ";
        static std::string       fullMessage;
        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }

}   // namespace settings