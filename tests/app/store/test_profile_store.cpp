// test_profile_store.cpp
//
// GoogleTest/GoogleMock-based tests for app::ProfileStore.
//
// Coverage:
//  - Constructor: loads profiles from service, initial state
//  - hasProfiles()
//  - getAllProfileNames(): returns names, ignores deleted
//  - isDirty(): reflects pending changes
//  - getProfile(id) / getProfile(name): finds profiles, ignores deleted
//  - profileExists(name) / profileExists(Profile)
//  - setActiveProfile() / unsetActiveProfile() / hasActiveProfile()
//  - getActiveProfile() / getActiveProfileName()
//  - addProfile(): new profile, duplicate name, restore deleted
//  - removeProfile(): soft-delete clean, hard-delete new, unset active
//  - reload(): refreshes from service, clears dirty, resets lost active profile
//  - commit(): create/update/remove service calls, makes store clean

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"
#include "services_api/i_profile_service.hpp"
#include "store/profile_store/profile_store.hpp"

using namespace app;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace
{

    class MockProfileService : public IProfileService
    {
       public:
        MOCK_METHOD(std::optional<Profile>, get, (ProfileId id), (const, override));
        MOCK_METHOD(std::vector<Profile>, getAll, (), (const, override));
        MOCK_METHOD(
            ProfileId,
            create,
            (const std::string& name, const std::optional<std::string>& email),
            (override)
        );
        MOCK_METHOD(
            void,
            update,
            (ProfileId                         id,
             const std::string&                newName,
             const std::optional<std::string>& newEmail),
            (override)
        );
        MOCK_METHOD(void, remove, (ProfileId id), (override));
    };

    Profile makeProfile(
        std::int64_t               id,
        const std::string&         name,
        std::optional<std::string> email = std::nullopt
    )
    {
        return Profile{ProfileId{id}, name, email};
    }

    drafts::ProfileDraft makeDraft(
        const std::string&         name,
        std::optional<std::string> email = std::nullopt
    )
    {
        return drafts::ProfileDraft{name, email};
    }

    class ProfileStoreTest : public ::testing::Test
    {
       protected:
        std::shared_ptr<NiceMock<MockProfileService>> service;
        std::unique_ptr<ProfileStore>                 store;

        void SetUp() override
        {
            service = std::make_shared<NiceMock<MockProfileService>>();
        }

        void createStore(std::vector<Profile> initialProfiles = {})
        {
            EXPECT_CALL(*service, getAll()).WillOnce(Return(initialProfiles));
            store = std::make_unique<ProfileStore>(service);
        }
    };

}   // namespace

// ============================================================
// Constructor / initial state
// ============================================================

TEST_F(ProfileStoreTest, ConstructorWithEmptyServiceHasNoProfiles)
{
    createStore();

    EXPECT_FALSE(store->hasProfiles());
    EXPECT_FALSE(store->isDirty());
    EXPECT_FALSE(store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, ConstructorLoadsProfilesFromService)
{
    createStore({makeProfile(1, "Alice"), makeProfile(2, "Bob")});

    EXPECT_TRUE(store->hasProfiles());
    EXPECT_FALSE(store->isDirty());
}

// ============================================================
// hasProfiles
// ============================================================

TEST_F(ProfileStoreTest, HasProfilesReturnsTrueWithExistingProfiles)
{
    createStore({makeProfile(1, "Alice")});
    EXPECT_TRUE(store->hasProfiles());
}

TEST_F(ProfileStoreTest, HasProfilesReturnsFalseWhenEmpty)
{
    createStore();
    EXPECT_FALSE(store->hasProfiles());
}

TEST_F(ProfileStoreTest, HasProfilesReturnsFalseAfterAllProfilesDeleted)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->hasProfiles());
}

// ============================================================
// getAllProfileNames
// ============================================================

TEST_F(ProfileStoreTest, GetAllProfileNamesReturnsAllNames)
{
    createStore({makeProfile(1, "Alice"), makeProfile(2, "Bob")});

    const auto names = store->getAllProfileNames();

    ASSERT_EQ(names.size(), 2u);
    EXPECT_EQ(names[0], "Alice");
    EXPECT_EQ(names[1], "Bob");
}

TEST_F(ProfileStoreTest, GetAllProfileNamesReturnsEmptyWhenNoProfiles)
{
    createStore();
    EXPECT_TRUE(store->getAllProfileNames().empty());
}

TEST_F(ProfileStoreTest, GetAllProfileNamesIgnoresDeletedProfiles)
{
    createStore({makeProfile(1, "Alice"), makeProfile(2, "Bob")});
    store->removeProfile(makeDraft("Alice"));

    const auto names = store->getAllProfileNames();

    ASSERT_EQ(names.size(), 1u);
    EXPECT_EQ(names[0], "Bob");
}

// ============================================================
// isDirty
// ============================================================

TEST_F(ProfileStoreTest, IsDirtyFalseAfterConstruction)
{
    createStore({makeProfile(1, "Alice")});
    EXPECT_FALSE(store->isDirty());
}

TEST_F(ProfileStoreTest, IsDirtyTrueAfterAddProfile)
{
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());
}

TEST_F(ProfileStoreTest, IsDirtyTrueAfterRemoveCleanProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());
}

TEST_F(ProfileStoreTest, IsDirtyFalseAfterAddAndRemoveNewProfile)
{
    // A New profile that is removed before commit is fully discarded,
    // leaving no dirty entries in the state map.
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());

    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->isDirty());
}

// ============================================================
// getProfile by ID
// ============================================================

TEST_F(ProfileStoreTest, GetProfileByIdReturnsProfileWhenFound)
{
    createStore({makeProfile(1, "Alice", "alice@example.com")});

    const auto result = store->getProfile(ProfileId{1});

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getName(), "Alice");
    EXPECT_EQ(result->getEmail(), "alice@example.com");
}

TEST_F(ProfileStoreTest, GetProfileByIdReturnsNulloptWhenNotFound)
{
    createStore();
    EXPECT_FALSE(store->getProfile(ProfileId{999}).has_value());
}

TEST_F(ProfileStoreTest, GetProfileByIdReturnsNulloptForDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->getProfile(ProfileId{1}).has_value());
}

// ============================================================
// getProfile by name
// ============================================================

TEST_F(ProfileStoreTest, GetProfileByNameReturnsProfileWhenFound)
{
    createStore({makeProfile(1, "Alice")});

    const auto result = store->getProfile("Alice");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getName(), "Alice");
    EXPECT_EQ(result->getId(), ProfileId{1});
}

TEST_F(ProfileStoreTest, GetProfileByNameReturnsNulloptWhenNotFound)
{
    createStore();
    EXPECT_FALSE(store->getProfile("Nonexistent").has_value());
}

TEST_F(ProfileStoreTest, GetProfileByNameReturnsNulloptForDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->getProfile("Alice").has_value());
}

// ============================================================
// profileExists
// ============================================================

TEST_F(ProfileStoreTest, ProfileExistsByNameReturnsTrueWhenExists)
{
    createStore({makeProfile(1, "Alice")});
    EXPECT_TRUE(store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ProfileExistsByNameReturnsFalseWhenNotExists)
{
    createStore();
    EXPECT_FALSE(store->profileExists("Nonexistent"));
}

TEST_F(ProfileStoreTest, ProfileExistsByNameReturnsFalseForDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ProfileExistsByProfileMatchesOnName)
{
    createStore({makeProfile(1, "Alice")});
    // Different ID, same name — should still find it by name
    const auto p = makeProfile(99, "Alice");
    EXPECT_TRUE(store->profileExists(p));
}

TEST_F(ProfileStoreTest, ProfileExistsByProfileReturnsFalseWhenNameNotFound)
{
    createStore();
    const auto p = makeProfile(1, "Alice");
    EXPECT_FALSE(store->profileExists(p));
}

// ============================================================
// setActiveProfile / unsetActiveProfile / hasActiveProfile
// ============================================================

TEST_F(ProfileStoreTest, HasActiveProfileFalseInitially)
{
    createStore();
    EXPECT_FALSE(store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfileByName)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");
    EXPECT_TRUE(store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, SetActiveProfileThrowsWhenProfileNotFound)
{
    createStore();
    EXPECT_THROW(store->setActiveProfile("Nonexistent"), std::runtime_error);
}

TEST_F(ProfileStoreTest, SetActiveProfileWithNulloptUnsetsActiveProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");
    EXPECT_TRUE(store->hasActiveProfile());

    store->setActiveProfile(std::nullopt);
    EXPECT_FALSE(store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, UnsetActiveProfileClearsActiveProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");

    store->unsetActiveProfile();

    EXPECT_FALSE(store->hasActiveProfile());
}

// ============================================================
// getActiveProfile / getActiveProfileName
// ============================================================

TEST_F(ProfileStoreTest, GetActiveProfileReturnsNulloptWhenNotSet)
{
    createStore();
    EXPECT_FALSE(store->getActiveProfile().has_value());
}

TEST_F(ProfileStoreTest, GetActiveProfileReturnsCorrectProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");

    const auto result = store->getActiveProfile();

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getName(), "Alice");
}

TEST_F(ProfileStoreTest, GetActiveProfileNameReturnsNulloptWhenNotSet)
{
    createStore();
    EXPECT_FALSE(store->getActiveProfileName().has_value());
}

TEST_F(ProfileStoreTest, GetActiveProfileNameReturnsCorrectName)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");
    EXPECT_EQ(store->getActiveProfileName(), "Alice");
}

// ============================================================
// addProfile
// ============================================================

TEST_F(ProfileStoreTest, AddProfileReturnsOkForNewProfile)
{
    createStore();
    const auto result = store->addProfile(makeDraft("Alice"));
    EXPECT_EQ(result, ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, AddProfileMakesProfileAvailable)
{
    createStore();
    store->addProfile(makeDraft("Alice", "alice@example.com"));

    ASSERT_TRUE(store->profileExists("Alice"));
    const auto profile = store->getProfile("Alice");
    ASSERT_TRUE(profile.has_value());
    EXPECT_EQ(profile->getEmail(), "alice@example.com");
}

TEST_F(ProfileStoreTest, AddProfileReturnsNameAlreadyExistsForDuplicateName)
{
    createStore({makeProfile(1, "Alice")});
    const auto result = store->addProfile(makeDraft("Alice"));
    EXPECT_EQ(result, ProfileStoreResult::NameAlreadyExists);
}

TEST_F(ProfileStoreTest, AddProfileRestoresDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_FALSE(store->profileExists("Alice"));

    const auto result = store->addProfile(makeDraft("Alice", "new@example.com"));

    EXPECT_EQ(result, ProfileStoreResult::Ok);
    EXPECT_TRUE(store->profileExists("Alice"));
    EXPECT_EQ(store->getProfile("Alice")->getEmail(), "new@example.com");
}

TEST_F(ProfileStoreTest, AddProfileMakesStoreDirty)
{
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());
}

// ============================================================
// removeProfile
// ============================================================

TEST_F(ProfileStoreTest, RemoveProfileReturnsOkForExistingProfile)
{
    createStore({makeProfile(1, "Alice")});
    const auto result = store->removeProfile(makeDraft("Alice"));
    EXPECT_EQ(result, ProfileStoreResult::Ok);
}

TEST_F(ProfileStoreTest, RemoveProfileReturnsProfileNotFoundForNonexistent)
{
    createStore();
    const auto result = store->removeProfile(makeDraft("Nonexistent"));
    EXPECT_EQ(result, ProfileStoreResult::ProfileNotFound);
}

TEST_F(ProfileStoreTest, RemoveProfileSoftDeletesCleanProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));

    EXPECT_FALSE(store->profileExists("Alice"));
    EXPECT_TRUE(store->isDirty());
}

TEST_F(ProfileStoreTest, RemoveProfileHardDeletesNewProfile)
{
    // A New profile removed before commit is discarded entirely — no dirty state.
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());

    store->removeProfile(makeDraft("Alice"));

    EXPECT_FALSE(store->profileExists("Alice"));
    EXPECT_FALSE(store->isDirty());
}

TEST_F(ProfileStoreTest, RemoveProfileUnsetsActiveProfileWhenActiveProfileIsRemoved)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");
    EXPECT_TRUE(store->hasActiveProfile());

    store->removeProfile(makeDraft("Alice"));

    EXPECT_FALSE(store->hasActiveProfile());
}

TEST_F(
    ProfileStoreTest,
    RemoveProfileDoesNotAffectActiveProfileWhenDifferentProfileRemoved
)
{
    createStore({makeProfile(1, "Alice"), makeProfile(2, "Bob")});
    store->setActiveProfile("Alice");

    store->removeProfile(makeDraft("Bob"));

    EXPECT_TRUE(store->hasActiveProfile());
    EXPECT_EQ(store->getActiveProfileName(), "Alice");
}

// ============================================================
// reload
// ============================================================

TEST_F(ProfileStoreTest, ReloadFetchesProfilesFromServiceAgain)
{
    createStore({makeProfile(1, "Alice")});

    EXPECT_CALL(*service, getAll())
        .WillOnce(Return(std::vector<Profile>{
            makeProfile(1, "Alice Updated"),
            makeProfile(2, "Bob"),
        }));

    store->reload();

    EXPECT_TRUE(store->profileExists("Alice Updated"));
    EXPECT_TRUE(store->profileExists("Bob"));
    EXPECT_FALSE(store->profileExists("Alice"));
}

TEST_F(ProfileStoreTest, ReloadClearsDirtyState)
{
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());

    EXPECT_CALL(*service, getAll()).WillOnce(Return(std::vector<Profile>{}));

    store->reload();

    EXPECT_FALSE(store->isDirty());
}

TEST_F(ProfileStoreTest, ReloadResetsActiveProfileIfNoLongerExists)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");
    EXPECT_TRUE(store->hasActiveProfile());

    EXPECT_CALL(*service, getAll()).WillOnce(Return(std::vector<Profile>{}));

    store->reload();

    EXPECT_FALSE(store->hasActiveProfile());
}

TEST_F(ProfileStoreTest, ReloadPreservesActiveProfileWhenStillExists)
{
    createStore({makeProfile(1, "Alice")});
    store->setActiveProfile("Alice");

    EXPECT_CALL(*service, getAll())
        .WillOnce(Return(std::vector<Profile>{makeProfile(1, "Alice")}));

    store->reload();

    EXPECT_TRUE(store->hasActiveProfile());
    EXPECT_EQ(store->getActiveProfileName(), "Alice");
}

// ============================================================
// commit
// ============================================================

TEST_F(ProfileStoreTest, CommitDoesNothingWhenClean)
{
    createStore({makeProfile(1, "Alice")});

    EXPECT_CALL(*service, create(_, _)).Times(0);
    EXPECT_CALL(*service, update(_, _, _)).Times(0);
    EXPECT_CALL(*service, remove(_)).Times(0);
    EXPECT_CALL(*service, getAll()).Times(0);

    store->commit();

    EXPECT_FALSE(store->isDirty());
}

TEST_F(ProfileStoreTest, CommitCallsServiceCreateForNewProfile)
{
    createStore();
    store->addProfile(makeDraft("Alice"));

    EXPECT_CALL(*service, create("Alice", std::optional<std::string>{std::nullopt}))
        .WillOnce(Return(ProfileId{1}));
    EXPECT_CALL(*service, getAll())
        .WillOnce(Return(std::vector<Profile>{makeProfile(1, "Alice")}));

    store->commit();
}

TEST_F(ProfileStoreTest, CommitCallsServiceRemoveForDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));

    EXPECT_CALL(*service, remove(ProfileId{1}));
    EXPECT_CALL(*service, getAll()).WillOnce(Return(std::vector<Profile>{}));

    store->commit();
}

TEST_F(ProfileStoreTest, CommitCallsServiceUpdateForRestoredProfile)
{
    // Soft-delete then re-add with new email → Modified state → service.update()
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    store->addProfile(makeDraft("Alice", "updated@example.com"));

    EXPECT_CALL(
        *service,
        update(
            ProfileId{1},
            std::string{"Alice"},
            std::optional<std::string>{"updated@example.com"}
        )
    );
    EXPECT_CALL(*service, getAll())
        .WillOnce(Return(std::vector<Profile>{
            makeProfile(1, "Alice", "updated@example.com"),
        }));

    store->commit();
}

TEST_F(ProfileStoreTest, CommitMakesStoreCleanAfterNewProfile)
{
    createStore();
    store->addProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());

    EXPECT_CALL(*service, create(_, _)).WillOnce(Return(ProfileId{1}));
    EXPECT_CALL(*service, getAll())
        .WillOnce(Return(std::vector<Profile>{makeProfile(1, "Alice")}));

    store->commit();

    EXPECT_FALSE(store->isDirty());
}

TEST_F(ProfileStoreTest, CommitMakesStoreCleanAfterDeletedProfile)
{
    createStore({makeProfile(1, "Alice")});
    store->removeProfile(makeDraft("Alice"));
    EXPECT_TRUE(store->isDirty());

    EXPECT_CALL(*service, remove(ProfileId{1}));
    EXPECT_CALL(*service, getAll()).WillOnce(Return(std::vector<Profile>{}));

    store->commit();

    EXPECT_FALSE(store->isDirty());
}
