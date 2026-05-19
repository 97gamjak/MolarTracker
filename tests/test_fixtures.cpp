#include "test_fixtures.hpp"

#include <filesystem>
#include <iostream>
#include <random>

namespace tests
{
    /**
     * @brief Construct a new TempDbFile object.
     *
     */
    TempDbFile::TempDbFile() : _path(_unique_db_path()) {}

    TempDbFile::~TempDbFile()
    {
        std::error_code errorCode;
        std::filesystem::remove(_path, errorCode);
    }

    /**
     * @brief Get the path to the temporary database file.
     *
     * @return const std::filesystem::path&
     */
    const std::filesystem::path& TempDbFile::path() const noexcept
    {
        return _path;
    }

    /**
     * @brief Generate a unique database file path.
     *
     * @return std::filesystem::path
     */
    std::filesystem::path TempDbFile::_unique_db_path()
    {
        namespace fs = std::filesystem;

        fs::path base;
        try
        {
            base = fs::temp_directory_path();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to access temp directory (" << e.what()
                      << "), using current directory\n";
            base = fs::current_path();
        }

        std::random_device random;
        const auto         random1 = static_cast<unsigned>(random());
        const auto         random2 = static_cast<unsigned>(random());

        return base /
               ("molartracker_account_repo_test_" + std::to_string(random1) +
                "_" + std::to_string(random2) + ".sqlite");
    }

}   // namespace tests