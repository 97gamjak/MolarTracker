#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <string>

#include "settings/params/param_container.hpp"
#include "settings/params/param_container_mixin.hpp"
#include "settings/params/param_core.hpp"

// ============================================================================
// Concrete container used in all tests below
// ============================================================================

namespace
{
    class TestContainer : public settings::ParamContainerMixin<TestContainer>
    {
       public:
        friend settings::ParamContainerMixin<TestContainer>;

        settings::ParamContainer _core{"tc", "Test Container", "TC desc"};
        settings::ParamCore<int> _intParam{"intKey", "Int", "An int"};
        settings::ParamCore<std::string> _strParam{"strKey", "Str", "A string"};

        template <typename Func>
        // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
        void forEachParam(Func&& func)
        {
            func(_intParam);
            func(_strParam);
        }

        template <typename Func>
        // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
        void forEachParam(Func&& func) const
        {
            func(_intParam);
            func(_strParam);
        }
    };
}   // namespace

// ============================================================================
// Metadata via ParamContainerMixin
// ============================================================================

TEST(ParamContainerMixin, GetKeyReturnsContainerKey)
{
    TestContainer testContainer;
    EXPECT_EQ(testContainer.getKey(), "tc");
}

TEST(ParamContainerMixin, GetTitleReturnsContainerTitle)
{
    TestContainer testContainer;
    EXPECT_EQ(testContainer.getTitle(), "Test Container");
}

TEST(ParamContainerMixin, GetDescriptionReturnsContainerDescription)
{
    TestContainer testContainer;
    EXPECT_EQ(testContainer.getDescription(), "TC desc");
}

// ============================================================================
// isDirty / commit
// ============================================================================

TEST(ParamContainerMixin, IsDirtyFalseOnFreshContainer)
{
    TestContainer testContainer;
    EXPECT_FALSE(testContainer.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueWhenIntParamChanged)
{
    const auto    paramValue = 42;
    TestContainer testContainer;
    testContainer._intParam.set(paramValue);
    EXPECT_TRUE(testContainer.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueWhenStrParamChanged)
{
    const auto    paramValue = std::string("hello");
    TestContainer testContainer;
    testContainer._strParam.set(paramValue);
    EXPECT_TRUE(testContainer.isDirty());
}

TEST(ParamContainerMixin, CommitClearsDirtyForAllParams)
{
    TestContainer testContainer;
    testContainer._intParam.set(1);
    testContainer._strParam.set(std::string("world"));
    EXPECT_TRUE(testContainer.isDirty());

    testContainer.commit();
    EXPECT_FALSE(testContainer.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueAfterCommitAndNewChange)
{
    TestContainer testContainer;
    const auto    paramValue = 10;
    testContainer._intParam.set(paramValue);
    testContainer.commit();
    EXPECT_FALSE(testContainer.isDirty());

    const auto paramValue2 = 20;
    testContainer._intParam.set(paramValue2);
    EXPECT_TRUE(testContainer.isDirty());
}

// ============================================================================
// resetToDefault
// ============================================================================

TEST(ParamContainerMixin, ResetToDefaultRestoresParamsWithDefaultConfigured)
{
    TestContainer testContainer;
    testContainer._intParam.setDefault(1);
    testContainer._intParam.set(42);
    testContainer._strParam.set(std::string("changed"));   // no default set

    testContainer.resetToDefault();

    EXPECT_EQ(testContainer._intParam.get(), 1);
    // _strParam has no configured default, so resetToDefault leaves it as-is
    EXPECT_EQ(testContainer._strParam.get(), std::string("changed"));
}

// ============================================================================
// toJson
// ============================================================================

TEST(ParamContainerMixin, ToJsonContainsBothParamKeys)
{
    TestContainer testContainer;
    const auto    paramValue = 7;
    testContainer._intParam.set(paramValue);
    testContainer._strParam.set(std::string("test"));

    auto json = testContainer.toJson();
    EXPECT_TRUE(json.contains("intKey"));
    EXPECT_TRUE(json.contains("strKey"));
}

TEST(ParamContainerMixin, ToJsonValuesMatchParams)
{
    TestContainer testContainer;
    const auto    paramValue = 99;
    testContainer._intParam.set(paramValue);
    testContainer._strParam.set(std::string("abc"));

    auto json  = testContainer.toJson();
    auto intJs = json["intKey"];
    EXPECT_EQ(intJs["value"].get<int>(), paramValue);
}

// ============================================================================
// fromJson
// ============================================================================

TEST(ParamContainerMixin, FromJsonRestoresParamValues)
{
    TestContainer source;
    const auto    paramValue = 123;
    source._intParam.set(paramValue);
    source._strParam.set(std::string("roundtrip"));
    auto json = source.toJson();

    TestContainer target;
    TestContainer::fromJson(json, target);

    EXPECT_EQ(target._intParam.get(), paramValue);
    EXPECT_EQ(target._strParam.get(), std::string("roundtrip"));
}

TEST(ParamContainerMixin, FromJsonMissingKeyLeavesParamUntouched)
{
    // Provide JSON with only one of the two param keys
    TestContainer source;
    const auto    paramValue = 50;
    source._intParam.set(paramValue);
    auto json = source.toJson();
    json.erase("strKey");   // remove strKey entry

    TestContainer target;
    target._strParam.set(std::string("original"));
    TestContainer::fromJson(json, target);

    EXPECT_EQ(target._intParam.get(), paramValue);
    // strKey was absent from JSON, so _strParam keeps its previous value
    EXPECT_EQ(target._strParam.get(), std::string("original"));
}

// ============================================================================
// subscribeToDirty
// ============================================================================

TEST(ParamContainerMixin, SubscribeToDirtyFiresWhenParamChanges)
{
    TestContainer testContainer;
    bool          lastDirty = false;

    auto conns = testContainer.subscribeToDirty(
        [&](const bool& value) { lastDirty = value; },
        nullptr
    );

    const auto paramValue = 77;
    testContainer._intParam.set(paramValue);
    EXPECT_TRUE(lastDirty);
}

TEST(ParamContainerMixin, SubscribeToDirtyFiresForStrParam)
{
    TestContainer testContainer;
    bool          lastDirty = false;

    auto conns = testContainer.subscribeToDirty(
        [&](const bool& value) { lastDirty = value; },
        nullptr
    );

    testContainer._strParam.set(std::string("changed"));
    EXPECT_TRUE(lastDirty);
}
