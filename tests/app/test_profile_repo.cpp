#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/migration/migration_runner.hpp"
#include "app/repos/profile_repo.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "orm/crud/crud_error.hpp"

namespace
{

    std::filesystem::path unique_db_path()
    {
        namespace fs = std::filesystem;

        fs::path base;
        try
        {
            base = fs::temp_directory_path();
        }
        catch (...)
        {
            base = fs::current_path();
        }

        std::random_device rd;
        const auto         r1 = static_cast<unsigned>(rd());
        const auto         r2 = static_cast<unsigned>(rd());

        return base / ("molartracker_profile_repo_test_" +
                       std::to_string(r1) + "_" + std::to_string(r2) +
                       ".sqlite");
    }

    class TempDbFile
    {
       public:
        TempDbFile() : _path(unique_db_path()) {}

        TempDbFile(const TempDbFile&)            = delete;
        TempDbFile& operator=(const TempDbFile&) = delete;
        TempDbFile(TempDbFile&&)                 = default;
        TempDbFile& operator=(TempDbFile&&)      = default;

        ~TempDbFile()
        {
            std::error_code ec;
            std::filesystem::remove(_path, ec);
        }

        [[nodiscard]] const std::filesystem::path& path() const noexcept
        {
            return _path;
        }

       private:
        std::filesystem::path _path;
    };

    class ProfileRepoTest : public ::testing::Test
    {
       protected:
        TempDbFile        _tempFile;
        db::Database      _db;
        app::ProfileRepo  _repo;

        ProfileRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            app::MigrationRunner{_db};
        }

        [[nodiscard]] ProfileId createProfile(
            const std::string&                name,
            const std::optional<std::string>& email = std::nullopt
        )
        {
            return _repo.create(name, email);
        }
    };

}   // namespace

TEST_F(ProfileRepoTest, Create_ReturnsValidId)
{
    const auto id = _repo.create("Alice", std::nullopt);

    EXPECT_GT(id.value(), 0);
}

TEST_F(ProfileRepoTest, Create_DuplicateNameThrows)
{
    static_cast<void>(_repo.create("Alice", std::nullopt));

    EXPECT_THROW(_repo.create("Alice", std::nullopt), orm::CrudException);
}

TEST_F(ProfileRepoTest, Create_WithEmail_PersistsEmail)
{
    const auto id = _repo.create("Bob", std::string{"bob@example.com"});

    const auto profile = _repo.get(id);
    ASSERT_TRUE(profile.has_value());
    ASSERT_TRUE(profile->getEmail().has_value());
    EXPECT_EQ(profile->getEmail().value(), "bob@example.com");
}

TEST_F(ProfileRepoTest, Create_WithoutEmail_EmailIsEmpty)
{
    const auto id = _repo.create("Alice", std::nullopt);

    const auto profile = _repo.get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_FALSE(profile->getEmail().has_value());
}

TEST_F(ProfileRepoTest, GetById_ReturnsNulloptForMissingId)
{
    const auto profile = _repo.get(ProfileId{9999});

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, GetById_ReturnsCorrectProfile)
{
    const auto id = _repo.create("Carol", std::string{"carol@example.com"});

    const auto profile = _repo.get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Carol");
    EXPECT_EQ(profile->getId(), id);
}

TEST_F(ProfileRepoTest, GetByName_ReturnsNulloptForMissingName)
{
    const auto profile = _repo.get(std::string{"nonexistent"});

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, GetByName_ReturnsCorrectProfile)
{
    const auto id = _repo.create("Dave", std::nullopt);

    const auto profile = _repo.get(std::string{"Dave"});
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getId(), id);
    EXPECT_EQ(profile->getName(), "Dave");
}

TEST_F(ProfileRepoTest, GetAll_EmptyWhenNoProfiles)
{
    const auto profiles = _repo.getAll();

    EXPECT_TRUE(profiles.empty());
}

TEST_F(ProfileRepoTest, GetAll_ReturnsAllCreatedProfiles)
{
    static_cast<void>(_repo.create("Alice", std::nullopt));
    static_cast<void>(_repo.create("Bob", std::nullopt));
    static_cast<void>(_repo.create("Carol", std::nullopt));

    const auto profiles = _repo.getAll();

    EXPECT_EQ(profiles.size(), 3u);
}

TEST_F(ProfileRepoTest, GetAll_ReturnsCorrectProfileData)
{
    const auto id =
        _repo.create("Eve", std::string{"eve@example.com"});

    const auto profiles = _repo.getAll();
    ASSERT_EQ(profiles.size(), 1u);

    EXPECT_EQ(profiles[0].getName(), "Eve");
    EXPECT_EQ(profiles[0].getId(), id);
    ASSERT_TRUE(profiles[0].getEmail().has_value());
    EXPECT_EQ(profiles[0].getEmail().value(), "eve@example.com");
}

TEST_F(ProfileRepoTest, Update_ChangesNameAndEmail)
{
    const auto id = _repo.create("Frank", std::nullopt);

    _repo.update(id, "Franklin", std::string{"frank@example.com"});

    const auto profile = _repo.get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Franklin");
    ASSERT_TRUE(profile->getEmail().has_value());
    EXPECT_EQ(profile->getEmail().value(), "frank@example.com");
}

TEST_F(ProfileRepoTest, Update_ClearsEmail)
{
    const auto id = _repo.create("Grace", std::string{"grace@example.com"});

    _repo.update(id, "Grace", std::nullopt);

    const auto profile = _repo.get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_FALSE(profile->getEmail().has_value());
}

TEST_F(ProfileRepoTest, Update_ThrowsForNonExistentId)
{
    EXPECT_THROW(
        _repo.update(ProfileId{9999}, "Ghost", std::nullopt),
        std::runtime_error
    );
}

TEST_F(ProfileRepoTest, Update_DuplicateNameThrows)
{
    const auto id1 = _repo.create("Heidi", std::nullopt);
    static_cast<void>(_repo.create("Ivan", std::nullopt));

    EXPECT_THROW(
        _repo.update(id1, "Ivan", std::nullopt),
        orm::CrudException
    );
}

TEST_F(ProfileRepoTest, Remove_DeletesProfile)
{
    const auto id = _repo.create("Judy", std::nullopt);

    _repo.remove(id);

    const auto profile = _repo.get(id);
    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, Remove_DoesNotAffectOtherProfiles)
{
    const auto id1 = _repo.create("Mallory", std::nullopt);
    const auto id2 = _repo.create("Niaj", std::nullopt);

    _repo.remove(id1);

    EXPECT_FALSE(_repo.get(id1).has_value());
    EXPECT_TRUE(_repo.get(id2).has_value());
}
