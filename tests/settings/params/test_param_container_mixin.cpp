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

        settings::ParamContainer     _core{"tc", "Test Container", "TC desc"};
        settings::ParamCore<int>     _intParam{"intKey", "Int", "An int"};
        settings::ParamCore<std::string> _strParam{
            "strKey",
            "Str",
            "A string"
        };

        template <typename Func>
        void forEachParam(Func&& func)
        {
            func(_intParam);
            func(_strParam);
        }

        template <typename Func>
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
    TestContainer tc;
    EXPECT_EQ(tc.getKey(), "tc");
}

TEST(ParamContainerMixin, GetTitleReturnsContainerTitle)
{
    TestContainer tc;
    EXPECT_EQ(tc.getTitle(), "Test Container");
}

TEST(ParamContainerMixin, GetDescriptionReturnsContainerDescription)
{
    TestContainer tc;
    EXPECT_EQ(tc.getDescription(), "TC desc");
}

// ============================================================================
// isDirty / commit
// ============================================================================

TEST(ParamContainerMixin, IsDirtyFalseOnFreshContainer)
{
    TestContainer tc;
    EXPECT_FALSE(tc.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueWhenIntParamChanged)
{
    TestContainer tc;
    tc._intParam.set(42);
    EXPECT_TRUE(tc.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueWhenStrParamChanged)
{
    TestContainer tc;
    tc._strParam.set(std::string("hello"));
    EXPECT_TRUE(tc.isDirty());
}

TEST(ParamContainerMixin, CommitClearsDirtyForAllParams)
{
    TestContainer tc;
    tc._intParam.set(1);
    tc._strParam.set(std::string("world"));
    EXPECT_TRUE(tc.isDirty());

    tc.commit();
    EXPECT_FALSE(tc.isDirty());
}

TEST(ParamContainerMixin, IsDirtyTrueAfterCommitAndNewChange)
{
    TestContainer tc;
    tc._intParam.set(10);
    tc.commit();
    EXPECT_FALSE(tc.isDirty());

    tc._intParam.set(20);
    EXPECT_TRUE(tc.isDirty());
}

// ============================================================================
// toJson
// ============================================================================

TEST(ParamContainerMixin, ToJsonContainsBothParamKeys)
{
    TestContainer tc;
    tc._intParam.set(7);
    tc._strParam.set(std::string("test"));

    auto json = tc.toJson();
    EXPECT_TRUE(json.contains("intKey"));
    EXPECT_TRUE(json.contains("strKey"));
}

TEST(ParamContainerMixin, ToJsonValuesMatchParams)
{
    TestContainer tc;
    tc._intParam.set(99);
    tc._strParam.set(std::string("abc"));

    auto json  = tc.toJson();
    auto intJs = json["intKey"];
    EXPECT_EQ(intJs["value"].get<int>(), 99);
}

// ============================================================================
// fromJson
// ============================================================================

TEST(ParamContainerMixin, FromJsonRestoresParamValues)
{
    TestContainer source;
    source._intParam.set(123);
    source._strParam.set(std::string("roundtrip"));
    auto json = source.toJson();

    TestContainer target;
    TestContainer::fromJson(json, target);

    EXPECT_EQ(target._intParam.get(), 123);
    EXPECT_EQ(target._strParam.get(), std::string("roundtrip"));
}

TEST(ParamContainerMixin, FromJsonMissingKeyLeavesParamUntouched)
{
    // Provide JSON with only one of the two param keys
    TestContainer source;
    source._intParam.set(50);
    auto json = source.toJson();
    json.erase("strKey");   // remove strKey entry

    TestContainer target;
    target._strParam.set(std::string("original"));
    TestContainer::fromJson(json, target);

    EXPECT_EQ(target._intParam.get(), 50);
    // strKey was absent from JSON, so _strParam keeps its previous value
    EXPECT_EQ(target._strParam.get(), std::string("original"));
}

// ============================================================================
// subscribeToDirty
// ============================================================================

TEST(ParamContainerMixin, SubscribeToDirtyFiresWhenParamChanges)
{
    TestContainer tc;
    bool          lastDirty = false;

    auto conns = tc.subscribeToDirty(
        [&](const bool& d) { lastDirty = d; },
        nullptr
    );

    tc._intParam.set(77);
    EXPECT_TRUE(lastDirty);
}

TEST(ParamContainerMixin, SubscribeToDirtyFiresForStrParam)
{
    TestContainer tc;
    bool          lastDirty = false;

    auto conns = tc.subscribeToDirty(
        [&](const bool& d) { lastDirty = d; },
        nullptr
    );

    tc._strParam.set(std::string("changed"));
    EXPECT_TRUE(lastDirty);
}
