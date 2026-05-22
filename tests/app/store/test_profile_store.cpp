#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/store/profile/exception.hpp"
#include "app/store/profile/profile_store.hpp"
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

TEST_F(ProfileStoreTest, HasProfilesFalseWhenEmpty)
{
    EXPECT_FALSE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, HasProfilesTrueAfterAdd)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    EXPECT_TRUE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, AddProfileSuccessReturnsOk)
{
    const auto result = _store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    );

    EXPECT_EQ(result, app::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, AddProfileDuplicateNameReturnsNameAlreadyExists)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    const auto result = _store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    );

    EXPECT_EQ(result, app::ProfileStoreResult::NameAlreadyExists);
}

TEST_F(ProfileStoreTest, RemoveProfileSuccessReturnsOk)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    const auto result =
        _store->removeProfile({.name = "Alice", .email = std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, RemoveProfileNotFoundReturnsProfileNotFound)
{
    const auto result =
        _store->removeProfile({.name = "NonExistent", .email = std::nullopt});

    EXPECT_EQ(result, app::ProfileStoreResult::ProfileNotFound);
}

TEST_F(ProfileStoreTest, GetProfileByNameReturnsProfile)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    const auto profile = _store->getProfile("Alice");

    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getName(), "Alice");
}

TEST_F(ProfileStoreTest, GetProfileByNameNulloptWhenNotFound)
{
    const auto profile = _store->getProfile("NonExistent");

    EXPECT_FALSE(profile.has_value());
}

TEST_F(ProfileStoreTest, GetAllProfileNamesEmptyWhenNoProfiles)
{
    const auto names = _store->getAllProfileNames();

    EXPECT_TRUE(names.empty());
}

TEST_F(ProfileStoreTest, GetAllProfileNamesReturnsAddedNames)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Bob", .email = std::nullopt}
    ));

    const auto names = _store->getAllProfileNames();

    EXPECT_EQ(names.size(), 2U);
}

TEST_F(ProfileStoreTest, HasActiveProfileFalseInitially)
{
    EXPECT_FALSE(_store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfileValidNameSetsActiveProfile)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();

    _store->setActiveProfile("Alice");

    EXPECT_TRUE(_store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfileInvalidNameThrows)
{
    EXPECT_THROW(
        _store->setActiveProfile("NonExistent"),
        app::ProfileStoreException
    );
}

TEST_F(ProfileStoreTest, GetActiveProfileNameNulloptInitially)
{
    EXPECT_FALSE(_store->getActiveProfileName().has_value());
}

TEST_F(ProfileStoreTest, GetActiveProfileNameReturnsNameAfterSet)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();
    _store->setActiveProfile("Alice");

    const auto name = _store->getActiveProfileName();

    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(name.value(), "Alice");
}

TEST_F(ProfileStoreTest, CommitNewProfileCallsServiceCreate)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(ProfileStoreTest, CommitDeletedProfileCallsServiceRemove)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();
    static_cast<void>(
        _store->removeProfile({.name = "Alice", .email = std::nullopt})
    );

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

TEST_F(ProfileStoreTest, ProfileExistsByNameTrueForExisting)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{.name = "Alice", .email = std::nullopt}
    ));

    EXPECT_TRUE(_store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ProfileExistsByNameFalseForNonExisting)
{
    EXPECT_FALSE(_store->profileExists("NonExistent"));
}
