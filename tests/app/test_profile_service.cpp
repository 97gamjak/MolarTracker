#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "db/database.hpp"
#include "domain/profile.hpp"
#include "orm/crud/crud_error.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/profile_repo.hpp"
#include "service/profile_service.hpp"
#include "test_fixtures.hpp"

namespace
{

    class ProfileServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                        _tempFile;
        db::Database                             _db;
        std::shared_ptr<repo::ProfileRepo>       _repo;
        std::shared_ptr<service::ProfileService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        ProfileServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::ProfileRepo>(_db)},
              _service{std::make_shared<service::ProfileService>(_repo)}
        {
            repo::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(ProfileServiceTest, CreateReturnsValidId)
{
    const auto id = _service->create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_GT(id.value(), 0);
}

TEST_F(ProfileServiceTest, CreateDuplicateNameThrows)
{
    static_cast<void>(_service->create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    EXPECT_THROW(
        static_cast<void>(_service->create(
            domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
        )),
        orm::CrudException
    );
}

TEST_F(ProfileServiceTest, GetByIdReturnsNulloptForMissingId)
{
    const auto profile = _service->get(ProfileId{9999});

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileServiceTest, GetByIdReturnsCorrectProfile)
{
    const auto id = _service->create(
        domain::Profile{
            ProfileId::invalid(),
            "Bob",
            std::string{"bob@example.com"}
        }
    );

    const auto profile = _service->get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Bob");
    EXPECT_EQ(profile->getId(), id);
}

TEST_F(ProfileServiceTest, GetAllEmptyWhenNoProfiles)
{
    const auto profiles = _service->getAll();

    EXPECT_TRUE(profiles.empty());
}

TEST_F(ProfileServiceTest, GetAllReturnsAllCreatedProfiles)
{
    static_cast<void>(_service->create(
        domain::Profile{ProfileId::invalid(), "Alice", std::nullopt}
    ));
    static_cast<void>(_service->create(
        domain::Profile{ProfileId::invalid(), "Bob", std::nullopt}
    ));

    const auto profiles = _service->getAll();

    EXPECT_EQ(profiles.size(), 2U);
}

TEST_F(ProfileServiceTest, UpdateChangesNameAndEmail)
{
    const auto id = _service->create(
        domain::Profile{ProfileId::invalid(), "Carol", std::nullopt}
    );

    _service->update(id, "Caroline", std::string{"carol@example.com"});

    const auto profile = _service->get(id);
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Caroline");
    ASSERT_TRUE(profile->getEmail().has_value());
    EXPECT_EQ(profile->getEmail().value(), "carol@example.com");
}

TEST_F(ProfileServiceTest, UpdateThrowsForNonExistentId)
{
    EXPECT_THROW(
        _service->update(ProfileId{9999}, "Ghost", std::nullopt),
        std::runtime_error
    );
}

TEST_F(ProfileServiceTest, RemoveDeletesProfile)
{
    const auto id = _service->create(
        domain::Profile{ProfileId::invalid(), "Dave", std::nullopt}
    );

    _service->remove(id);

    EXPECT_FALSE(_service->get(id).has_value());
}

TEST_F(ProfileServiceTest, RemoveDoesNotAffectOtherProfiles)
{
    const auto id1 = _service->create(
        domain::Profile{ProfileId::invalid(), "Eve", std::nullopt}
    );
    const auto id2 = _service->create(
        domain::Profile{ProfileId::invalid(), "Frank", std::nullopt}
    );

    _service->remove(id1);

    EXPECT_FALSE(_service->get(id1).has_value());
    EXPECT_TRUE(_service->get(id2).has_value());
}
