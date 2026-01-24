#ifndef __CONFIG__CONSTANTS_HPP__
#define __CONFIG__CONSTANTS_HPP__

#include <filesystem>

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

    [[nodiscard]] std::filesystem::path get_config_path() const;
    [[nodiscard]] std::filesystem::path get_data_path() const;
    [[nodiscard]] std::filesystem::path get_database_path() const;

   private:
    Constants();
};

#endif   // __CONFIG__CONSTANTS_HPP__