#ifndef __CONFIG__CONSTANTS_HPP__
#define __CONFIG__CONSTANTS_HPP__

#include <filesystem>

/**
 * @brief Application-wide constants and paths, this is a singleton class
 *
 */
class Constants
{
   private:
    static constexpr const char* _databaseFile = "molar_tracker.db";

    std::filesystem::path _configPath;
    std::filesystem::path _dataPath;

   public:
    // clang-format off
    static constexpr const char* APP_NAME = "Molar Tracker";
    static constexpr const char* DIR_PREFIX = "molartracker";
    static constexpr const char* DESKTOP_APP_NAME = MOLARTRACKER_DESKTOP_APP_NAME;
    // clang-format on

   public:
    static Constants& getInstance();

    [[nodiscard]] std::filesystem::path getConfigPath() const;
    [[nodiscard]] std::filesystem::path getDataPath() const;
    [[nodiscard]] std::filesystem::path getDatabasePath() const;

   private:
    Constants();
};

#endif   // __CONFIG__CONSTANTS_HPP__