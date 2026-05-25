#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "drafts/profile_draft.hpp"
#include "mock_services.hpp"
#include "store/profile/exception.hpp"
#include "store/profile/profile_store.hpp"

namespace
{

    class ProfileStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockProfileService> _mockService;
        std::unique_ptr<store::ProfileStore>       _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        ProfileStoreTest()
            : _mockService{std::make_shared<tests::MockProfileService>()}
        {
            _store = std::make_unique<store::ProfileStore>(_mockService);
        }

        void rebuildStore()
        {
            _store = std::make_unique<store::ProfileStore>(_mockService);
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
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    EXPECT_TRUE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, AddProfileSuccessReturnsOk)
{
    const auto result = _store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_EQ(result, store::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, AddProfileDuplicateNameReturnsNameAlreadyExists)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    const auto result = _store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_EQ(result, store::ProfileStoreResult::NameAlreadyExists);
}

TEST_F(ProfileStoreTest, RemoveProfileSuccessReturnsOk)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    const auto result = _store->removeProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    );

    EXPECT_EQ(result, store::ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, RemoveProfileNotFoundReturnsProfileNotFound)
{
    const auto result = _store->removeProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "NonExistent", std::nullopt}
    );

    EXPECT_EQ(result, store::ProfileStoreResult::ProfileNotFound);
}

TEST_F(ProfileStoreTest, GetAllProfileNamesEmptyWhenNoProfiles)
{
    const auto names = _store->getAllProfileNames();

    EXPECT_TRUE(names.empty());
}

TEST_F(ProfileStoreTest, GetAllProfileNamesReturnsAddedNames)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Bob", std::nullopt}
    ));

    const auto names = _store->getAllProfileNames();

    EXPECT_EQ(names.size(), 2U);
}

TEST_F(ProfileStoreTest, SetActiveProfileInvalidNameRetrnsError)
{
    EXPECT_EQ(
        _store->setActiveProfile("NonExistent"),
        store::ProfileStoreResult::ProfileNotFound
    );
}

TEST_F(ProfileStoreTest, CommitNewProfileCallsServiceCreate)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(ProfileStoreTest, CommitDeletedProfileCallsServiceRemove)
{
    _mockService->addTestProfile("Alice");
    rebuildStore();
    static_cast<void>(_store->removeProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    _store->commit();

    EXPECT_EQ(_mockService->removeCallCount, 1);
}

TEST_F(ProfileStoreTest, LoadsProfilesFromServiceOnConstruction)
{
    _mockService->addTestProfile("Alice");
    _mockService->addTestProfile("Bob");
    rebuildStore();

    EXPECT_TRUE(_store->hasProfiles());
}

TEST_F(ProfileStoreTest, ProfileExistsByNameTrueForExisting)
{
    static_cast<void>(_store->addProfile(
        drafts::ProfileDraft{ProfileId::invalid(), "Alice", std::nullopt}
    ));

    EXPECT_TRUE(_store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ProfileExistsByNameFalseForNonExisting)
{
    EXPECT_FALSE(_store->profileExists("NonExistent"));
}
