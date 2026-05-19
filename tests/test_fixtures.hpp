#ifndef __TESTS__TEST_FIXTURES_HPP__
#define __TESTS__TEST_FIXTURES_HPP__

#include <filesystem>

namespace tests
{

    class TempDbFile
    {
       private:
        std::filesystem::path _path;

       public:
        TempDbFile();

        TempDbFile(const TempDbFile&)            = delete;
        TempDbFile& operator=(const TempDbFile&) = delete;
        TempDbFile(TempDbFile&&)                 = default;
        TempDbFile& operator=(TempDbFile&&)      = default;

        ~TempDbFile();

        [[nodiscard]] const std::filesystem::path& path() const noexcept;

       private:
        [[nodiscard]]
        static std::filesystem::path _unique_db_path();
    };

}   // namespace tests
#endif   // __TESTS__TEST_FIXTURES_HPP__
