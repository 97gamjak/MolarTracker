#ifndef __VCS__INCLUDE__VCS__UPDATE_INSTALLER_HPP__
#define __VCS__INCLUDE__VCS__UPDATE_INSTALLER_HPP__

#include <filesystem>

#include "error/update_error.hpp"

namespace vcs
{

    /**
     * @brief Extracts a downloaded release asset and applies it in place of
     * the running installation.
     *
     * The Linux path (extract a bare binary, atomically swap it, relaunch)
     * is exercised end-to-end in CI/dev on Linux. The Windows path (extract
     * a full deploy directory, hand off to a generated helper script that
     * waits for this process to exit before copying files and relaunching)
     * follows the same design but has not been executed on a real Windows
     * machine — verify manually before it ships in a release.
     *
     */
    class UpdateInstaller
    {
       public:
        /**
         * @brief Extract a downloaded release archive into a fresh staging
         * directory and verify the expected executable is present.
         *
         * @param archivePath Path to the downloaded .tar.gz/.zip archive
         * @return UpdateResult<std::filesystem::path> The staging directory
         * containing the extracted update, or an error
         */
        [[nodiscard]]
        static UpdateResult<std::filesystem::path> stageDownloadedAsset(
            const std::filesystem::path& archivePath
        );

        /**
         * @brief Apply a staged update in place of the running installation
         * and relaunch the application.
         *
         * On success, the caller is responsible for quitting the current
         * process (e.g. QCoreApplication::quit()) — this function only
         * arranges for the new version to take over and start.
         *
         * @param stagingDir Directory previously returned by
         * stageDownloadedAsset()
         * @return UpdateResult<void>
         */
        [[nodiscard]]
        static UpdateResult<void> applyUpdateAndRestart(
            const std::filesystem::path& stagingDir
        );

       private:
        [[nodiscard]]
        static UpdateResult<std::filesystem::path> _prepareStagingDir();
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__UPDATE_INSTALLER_HPP__
