#include "vcs/update_installer.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>

#include "config/constants/constants.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("VCS.UpdateInstaller");

namespace vcs
{
    namespace
    {
#if defined(_WIN32)
        constexpr const char* _stagedExecutableName = "MolarTracker.exe";
#else
        constexpr const char* _stagedExecutableName = "MolarTracker";
#endif

        /**
         * @brief Run a process to completion and report whether it exited
         * cleanly (started, ran to completion, exit code 0).
         *
         * @param program The program to run
         * @param arguments The arguments to pass to the program
         * @return true If the process started and exited cleanly
         */
        bool _runProcessToCompletion(
            const QString&     program,
            const QStringList& arguments
        )
        {
            QProcess process;
            process.start(program, arguments);
            if (!process.waitForStarted())
                return false;

            process.waitForFinished(-1);
            return process.exitStatus() == QProcess::NormalExit &&
                   process.exitCode() == 0;
        }
    }   // namespace

    /**
     * @brief Prepare a fresh, empty staging directory for extracting a
     * downloaded update into.
     *
     * @return UpdateResult<std::filesystem::path>
     */
    UpdateResult<std::filesystem::path> UpdateInstaller::_prepareStagingDir()
    {
        const auto stagingRoot =
            Constants::getInstance().getUpdateStagingPath();
        const auto extractDir = stagingRoot / "extract";

        std::error_code errorCode;
        std::filesystem::remove_all(extractDir, errorCode);
        std::filesystem::create_directories(extractDir, errorCode);

        if (errorCode)
        {
            return UpdateError{
                UpdateErrorType::StagingVerificationFailed,
                "failed to prepare staging directory: " + errorCode.message()
            };
        }

        return extractDir;
    }

    /**
     * @brief Extract a downloaded release archive and verify it contains the
     * expected executable.
     *
     * @param archivePath Path to the downloaded .tar.gz/.zip archive
     * @return UpdateResult<std::filesystem::path>
     */
    UpdateResult<std::filesystem::path> UpdateInstaller::stageDownloadedAsset(
        const std::filesystem::path& archivePath
    )
    {
        const auto staged = _prepareStagingDir();
        if (!staged)
            return staged.error();

        const auto& stagingDir = *staged;
        const auto  archiveQString =
            QString::fromStdString(archivePath.string());
        const auto stagingQString = QString::fromStdString(stagingDir.string());

#if defined(_WIN32)
        const bool extracted = _runProcessToCompletion(
            "powershell",
            {
                "-NoProfile",
                "-Command",
                QString(
                    "Expand-Archive -LiteralPath '%1' -DestinationPath '%2' "
                    "-Force"
                )
                    .arg(archiveQString, stagingQString),
            }
        );
#else
        const bool extracted = _runProcessToCompletion(
            "tar",
            {"xzf", archiveQString, "-C", stagingQString}
        );
#endif

        if (!extracted)
        {
            return UpdateError{
                UpdateErrorType::ExtractionFailed,
                "failed to extract downloaded update archive"
            };
        }

        const auto expectedExecutable = stagingDir / _stagedExecutableName;
        if (!std::filesystem::exists(expectedExecutable))
        {
            return UpdateError{
                UpdateErrorType::StagingVerificationFailed,
                "extracted update does not contain " +
                    std::string{_stagedExecutableName}
            };
        }

        LOG_INFO("Staged update at " + stagingDir.string());
        return stagingDir;
    }

#if defined(_WIN32)

    // NOTE: this path follows the same design as the Linux implementation
    // below but has not been executed on a real Windows machine — there is
    // no Windows environment available in this repository's sandbox for
    // testing. Verify manually on Windows before this ships in a release.
    /**
     * @brief Apply a staged update on Windows.
     *
     * A running .exe and its DLLs cannot be overwritten while the process
     * holds them open, so this hands off to a generated helper script that
     * waits for this process to exit, mirrors the staged directory over the
     * install directory, relaunches the app, then deletes itself.
     *
     * @param stagingDir Directory previously returned by
     * stageDownloadedAsset()
     * @return UpdateResult<void>
     */
    UpdateResult<void> UpdateInstaller::applyUpdateAndRestart(
        const std::filesystem::path& stagingDir
    )
    {
        const QFileInfo currentExe(QCoreApplication::applicationFilePath());
        const QString   installDir = currentExe.absolutePath();
        const qint64    pid        = QCoreApplication::applicationPid();

        QTemporaryFile scriptFile(
            QDir::tempPath() + "/molartracker_update_XXXXXX.bat"
        );
        scriptFile.setAutoRemove(false);
        if (!scriptFile.open())
        {
            return UpdateError{
                UpdateErrorType::ApplyFailed,
                "failed to create update helper script"
            };
        }

        const QString script =
            QString(
                "@echo off\r\n"
                ":wait\r\n"
                "tasklist /FI \"PID eq %1\" 2>NUL | find \"%1\" >NUL\r\n"
                "if not errorlevel 1 (\r\n"
                "  timeout /T 1 /NOBREAK >NUL\r\n"
                "  goto wait\r\n"
                ")\r\n"
                "robocopy \"%2\" \"%3\" /E /IS /IT /NFL /NDL /NJH /NJS\r\n"
                "start \"\" \"%3\\MolarTracker.exe\"\r\n"
                "del \"%%~f0\"\r\n"
            )
                .arg(QString::number(pid))
                .arg(QString::fromStdString(stagingDir.string()))
                .arg(installDir);

        scriptFile.write(script.toUtf8());
        const QString scriptPath = scriptFile.fileName();
        scriptFile.close();

        const bool started =
            QProcess::startDetached("cmd.exe", {"/c", scriptPath});

        if (!started)
        {
            return UpdateError{
                UpdateErrorType::ApplyFailed,
                "failed to launch update helper script"
            };
        }

        LOG_INFO("Launched update helper script, awaiting relaunch");
        return {};
    }

#else

    /**
     * @brief Apply a staged update on Linux.
     *
     * Backs up the running executable to "<path>.old" (same directory/
     * filesystem, so the rename is atomic and safe even while this process
     * is still executing from it), moves the staged binary into place,
     * marks it executable, and relaunches. Any failure before the final
     * rename rolls back from the backup without touching the running
     * process.
     *
     * @param stagingDir Directory previously returned by
     * stageDownloadedAsset()
     * @return UpdateResult<void>
     */
    UpdateResult<void> UpdateInstaller::applyUpdateAndRestart(
        const std::filesystem::path& stagingDir
    )
    {
        const std::filesystem::path currentExe =
            QCoreApplication::applicationFilePath().toStdString();
        const std::filesystem::path newBinary =
            stagingDir / _stagedExecutableName;
        const std::filesystem::path backupPath = currentExe.string() + ".old";

        std::error_code errorCode;
        std::filesystem::remove(backupPath, errorCode);

        std::filesystem::rename(currentExe, backupPath, errorCode);
        if (errorCode)
        {
            return UpdateError{
                UpdateErrorType::ApplyFailed,
                "failed to back up the running executable: " +
                    errorCode.message()
            };
        }

        std::filesystem::rename(newBinary, currentExe, errorCode);
        if (errorCode)
        {
            std::error_code rollbackError;
            std::filesystem::rename(backupPath, currentExe, rollbackError);

            return UpdateError{
                UpdateErrorType::ApplyFailed,
                "failed to install the new executable, rolled back: " +
                    errorCode.message()
            };
        }

        std::filesystem::permissions(
            currentExe,
            std::filesystem::perms::owner_exec |
                std::filesystem::perms::group_exec |
                std::filesystem::perms::others_exec,
            std::filesystem::perm_options::add,
            errorCode
        );

        const bool started = QProcess::startDetached(
            QString::fromStdString(currentExe.string()),
            {}
        );

        if (!started)
        {
            std::error_code rollbackError;
            std::filesystem::rename(currentExe, newBinary, rollbackError);
            std::filesystem::rename(backupPath, currentExe, rollbackError);

            return UpdateError{
                UpdateErrorType::ApplyFailed,
                "failed to relaunch the updated application, rolled back"
            };
        }

        std::filesystem::remove(backupPath, errorCode);

        LOG_INFO("Applied update, relaunched from " + currentExe.string());
        return {};
    }

#endif

}   // namespace vcs
