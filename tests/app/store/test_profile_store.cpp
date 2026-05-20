#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/store/profile/exception.hpp"
#include "app/store/profile/profile_store.hpp"
#include "config/id_types.hpp"
#include "drafts/profile_draft.hpp"
#include "mock_services.hpp"

namespace
{

    class ProfileStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockProfileService> _mockService;
        std::unique_ptr<app::ProfileStore>         _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        ProfileStoreTest()
            : _mockService{std::make_shared<tests::MockProfileService>()}
        {
            _store = std::make_unique<app::ProfileStore>(_mockService);
        }

        void rebuildStore()
        {
            _store = std::make_unique<app::ProfileStore>(_mockService);
        }
    };

}   // namespace

TEST_F(ProfileStoreTest, HasProfiles_FalseWhenEmpty)
{
    EXPECT_FALSE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, HasProfiles_TrueAfterAdd)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    EXPECT_TRUE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, AddProfile_Success_ReturnsOk)
{
    const auto result = _store->addProfile({"Alice", std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, AddProfile_DuplicateName_ReturnsNameAlreadyExists)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    const auto result = _store->addProfile({"Alice", std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::NameAlreadyExists);
}

TEST_F(ProfileStoreTest, RemoveProfile_Success_ReturnsOk)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    const auto result = _store->removeProfile({"Alice", std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, RemoveProfile_NotFound_ReturnsProfileNotFound)
{
    const auto result = _store->removeProfile({"NonExistent", std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::ProfileNotFound);
}

TEST_F(ProfileStoreTest, GetProfile_ByName_ReturnsProfile)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    const auto profile = _store->getProfile("Alice");

    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Alice");
}

TEST_F(ProfileStoreTest, GetProfile_ByName_NulloptWhenNotFound)
{
    const auto profile = _store->getProfile("NonExistent");

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileStoreTest, GetAllProfileNames_EmptyWhenNoProfiles)
{
    const auto names = _store->getAllProfileNames();

    EXPECT_TRUE(names.empty());
}

TEST_F(ProfileStoreTest, GetAllProfileNames_ReturnsAddedNames)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));
    static_cast<void>(_store->addProfile({"Bob", std::nullopt}));

    const auto names = _store->getAllProfileNames();

    EXPECT_EQ(names.size(), 2u);
}

TEST_F(ProfileStoreTest, HasActiveProfile_FalseInitially)
{
    EXPECT_FALSE(_store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfile_ValidName_SetsActiveProfile)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();

    _store->setActiveProfile("Alice");

    EXPECT_TRUE(_store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfile_InvalidName_Throws)
{
    EXPECT_THROW(
        _store->setActiveProfile("NonExistent"),
        app::ProfileStoreException
    );
}

TEST_F(ProfileStoreTest, GetActiveProfileName_NulloptInitially)
{
    EXPECT_FALSE(_store->getActiveProfileName().has_value());
}

TEST_F(ProfileStoreTest, GetActiveProfileName_ReturnsName_AfterSet)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();
    _store->setActiveProfile("Alice");

    const auto name = _store->getActiveProfileName();

    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(name.value(), "Alice");
}

TEST_F(ProfileStoreTest, Commit_NewProfile_CallsServiceCreate)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(ProfileStoreTest, Commit_DeletedProfile_CallsServiceRemove)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();
    static_cast<void>(_store->removeProfile({"Alice", std::nullopt}));

    _store->commit();

    EXPECT_EQ(_mockService->removeCallCount, 1);
}

TEST_F(ProfileStoreTest, LoadsProfilesFromServiceOnConstruction)
{
    _mockService->addTestProfile("Alice");
    _mockService->addTestProfile("Bob");
    rebuildStore();

    EXPECT_TRUE(_store->hasProfiles());
    EXPECT_TRUE(_store->getProfile("Alice").has_value());
    EXPECT_TRUE(_store->getProfile("Bob").has_value());
}

TEST_F(ProfileStoreTest, ProfileExists_ByName_TrueForExisting)
{
    static_cast<void>(_store->addProfile({"Alice", std::nullopt}));

    EXPECT_TRUE(_store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ProfileExists_ByName_FalseForNonExisting)
{
    EXPECT_FALSE(_store->profileExists("NonExistent"));
}
