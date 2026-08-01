#include <gtest/gtest.h>

#include <cassert>
#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "db/database.hpp"
#include "domain/profile.hpp"
#include "orm/crud/crud_error.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/profile_repo.hpp"
#include "test_fixtures.hpp"

namespace
{

    class ProfileRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile _tempFile;
        db::Database      _db;
        repo::ProfileRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        ProfileRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            repo::MigrationRunner{_db};
        }

        [[nodiscard]] ProfileId createProfile(const domain::Profile& profile)
        {
            return _repo.create(profile).value();
        }
    };

}   // namespace

TEST_F(ProfileRepoTest, CreateReturnsValidId)
{
    const auto id = _repo.create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_GT(id.value(), 0);
}

TEST_F(ProfileRepoTest, CreateDuplicateNameThrows)
{
    auto result = _repo.create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_THROW(
        result = _repo.create(
            domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
        ),
        orm::CrudException
    );
}

TEST_F(ProfileRepoTest, CreateWithEmailPersistsEmail)
{
    const auto id = _repo.create(
        domain::Profile{
            ProfileId::invalid(),
            "Bob",
            std::string{"bob@example.com"}
        }
    );

    const auto profile = _repo.get(id.value());
    ASSERT_TRUE(profile.has_value());
    ASSERT_TRUE(profile->getEmail().has_value());
    EXPECT_EQ(profile->getEmail().value(), "bob@example.com");
}

TEST_F(ProfileRepoTest, CreateWithoutEmailEmailIsEmpty)
{
    const auto id = _repo.create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    );

    const auto profile = _repo.get(id.value());
    ASSERT_TRUE(profile.has_value());
    EXPECT_FALSE(profile->getEmail().has_value());
}

TEST_F(ProfileRepoTest, GetByIdReturnsNulloptForMissingId)
{
    const auto profile = _repo.get(ProfileId{9999});

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, GetByIdReturnsCorrectProfile)
{
    const auto id = _repo.create(
        domain::Profile{
            ProfileId::invalid(),
            "Carol",
            std::string{"carol@example.com"}
        }
    );

    const auto profile = _repo.get(id.value());
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Carol");
    EXPECT_EQ(profile->getId(), id.value());
}

TEST_F(ProfileRepoTest, GetByNameReturnsNulloptForMissingName)
{
    const auto profile = _repo.get(std::string{"nonexistent"});

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, GetByNameReturnsCorrectProfile)
{
    const auto id = _repo.create(
        domain::Profile{ProfileId::invalid(), "Dave", std::nullopt}
    );

    const auto profile = _repo.get(std::string{"Dave"});
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getId(), id.value());
    EXPECT_EQ(profile->getName(), "Dave");
}

TEST_F(ProfileRepoTest, GetAllEmptyWhenNoProfiles)
{
    const auto profiles = _repo.getAll();

    EXPECT_TRUE(profiles.empty());
}

TEST_F(ProfileRepoTest, GetAllReturnsAllCreatedProfiles)
{
    static_cast<void>(_repo.create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    ));
    static_cast<void>(
        _repo.create(domain::Profile{ProfileId::invalid(), "Bob", std::nullopt})
    );
    static_cast<void>(_repo.create(
        domain::Profile{ProfileId::invalid(), "Carol", std::nullopt}
    ));

    const auto profiles = _repo.getAll();

    EXPECT_EQ(profiles.size(), 3U);
}

TEST_F(ProfileRepoTest, GetAllReturnsCorrectProfileData)
{
    const auto id = _repo.create(
        domain::Profile{
            ProfileId::invalid(),
            "Eve",
            std::string{"eve@example.com"}
        }
    );

    const auto profiles = _repo.getAll();
    ASSERT_EQ(profiles.size(), 1U);

    EXPECT_EQ(profiles[0].getName(), "Eve");
    EXPECT_EQ(profiles[0].getId(), id.value());
    ASSERT_TRUE(profiles[0].getEmail().has_value());
    EXPECT_EQ(profiles[0].getEmail().value(), "eve@example.com");
}

TEST_F(ProfileRepoTest, UpdateChangesNameAndEmail)
{
    const auto id = _repo.create(
        domain::Profile{ProfileId::invalid(), "Frank", std::nullopt}
    );

    _repo.update(id.value(), "Franklin", std::string{"frank@example.com"});

    const auto profile = _repo.get(id.value());
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Franklin");
    ASSERT_TRUE(profile->getEmail().has_value());
    EXPECT_EQ(profile->getEmail().value(), "frank@example.com");
}

TEST_F(ProfileRepoTest, UpdateClearsEmail)
{
    const auto id = _repo.create(
        domain::Profile{
            ProfileId::invalid(),
            "Grace",
            std::string{"grace@example.com"}
        }
    );

    _repo.update(id.value(), "Grace", std::nullopt);

    const auto profile = _repo.get(id.value());
    ASSERT_TRUE(profile.has_value());
    EXPECT_FALSE(profile->getEmail().has_value());
}

TEST_F(ProfileRepoTest, UpdateThrowsForNonExistentId)
{
    EXPECT_THROW(
        _repo.update(ProfileId{9999}, "Ghost", std::nullopt),
        std::runtime_error
    );
}

TEST_F(ProfileRepoTest, UpdateDuplicateNameThrows)
{
    const auto id1 = _repo.create(
        domain::Profile{ProfileId::invalid(), "Heidi", std::nullopt}
    );
    static_cast<void>(_repo.create(
        domain::Profile{ProfileId::invalid(), "Ivan", std::nullopt}
    ));

    EXPECT_THROW(
        _repo.update(id1.value(), "Ivan", std::nullopt),
        orm::CrudException
    );
}

TEST_F(ProfileRepoTest, RemoveDeletesProfile)
{
    const auto id = _repo.create(
        domain::Profile{ProfileId::invalid(), "Judy", std::nullopt}
    );

    _repo.remove(id.value());

    const auto profile = _repo.get(id.value());
    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileRepoTest, RemoveDoesNotAffectOtherProfiles)
{
    const auto id1 = _repo.create(
        domain::Profile{ProfileId::invalid(), "Mallory", std::nullopt}
    );
    const auto id2 = _repo.create(
        domain::Profile{ProfileId::invalid(), "Nina", std::nullopt}
    );

    _repo.remove(id1.value());

    EXPECT_FALSE(_repo.get(id1.value()).has_value());
    EXPECT_TRUE(_repo.get(id2.value()).has_value());
}
