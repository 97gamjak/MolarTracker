// tests/orm/binder_test.cpp

#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "mstd/enum.hpp"
#include "orm/binder.hpp"

namespace
{
    // ------------------------------------------------------------
    // Fake statement used by binder tests
    // ------------------------------------------------------------
    struct FakeStatement
    {
        // "bound" parameter storage
        struct BoundInt64
        {
            int          index;
            std::int64_t value;
        };
        struct BoundDouble
        {
            int    index;
            double value;
        };
        struct BoundText
        {
            int         index;
            std::string value;
        };

        std::optional<BoundInt64>  last_int64_bind;
        std::optional<BoundDouble> last_double_bind;
        std::optional<BoundText>   last_text_bind;

        // column storage
        std::unordered_map<int, std::int64_t> col_i64;
        std::unordered_map<int, double>       col_dbl;
        std::unordered_map<int, std::string>  col_txt;

        // binder API surface
        void bindInt64(int index, std::int64_t value)
        {
            last_int64_bind = BoundInt64{.index = index, .value = value};
        }

        void bindDouble(int index, double value)
        {
            last_double_bind = BoundDouble{.index = index, .value = value};
        }

        void bindText(int index, std::string const& value)
        {
            last_text_bind = BoundText{.index = index, .value = value};
        }

        [[nodiscard]] std::int64_t columnInt64(int col) const
        {
            auto it = col_i64.find(col);
            if (it == col_i64.end())
                throw std::runtime_error("FakeStatement: missing int64 column");
            return it->second;
        }

        [[nodiscard]] double columnDouble(int col) const
        {
            auto it = col_dbl.find(col);
            if (it == col_dbl.end())
            {
                throw std::runtime_error(
                    "FakeStatement: missing double column"
                );
            }
            return it->second;
        }

        [[nodiscard]] std::string columnText(int col) const
        {
            auto it = col_txt.find(col);
            if (it == col_txt.end())
                throw std::runtime_error("FakeStatement: missing text column");
            return it->second;
        }
    };

    // ------------------------------------------------------------
    // Strong-id test type (must satisfy orm::strong_id concept)
    // If your project already has a StrongId utility, use it instead.
    // ------------------------------------------------------------
    struct TestId
    {
        std::int64_t _v;

        [[nodiscard]] std::int64_t value() const noexcept { return _v; }

        static TestId from(std::int64_t value) noexcept
        {
            return TestId{value};
        }

        friend bool operator==(TestId lhs, TestId rhs) noexcept
        {
            return lhs._v == rhs._v;
        }
    };

    // NOLINTBEGIN
#define TEST_ENUM_LIST(X) \
    X(Alpha)              \
    X(Beta)

    MSTD_ENUM(TestEnum, std::uint8_t, TEST_ENUM_LIST);
    // NOLINTEND

}   // namespace

// ------------------------------------------------------------
// NOTE: these tests assume your orm::strong_id concept accepts TestId.
// If it doesn't (e.g., it requires a specific base type), replace TestId
// with your project’s StrongId type for the strong-id test cases.
// ------------------------------------------------------------

TEST(OrmBinder, Int64BindAndRead)
{
    FakeStatement statement;

    orm::binder<FakeStatement, std::int64_t>::bind(statement, 3, 123456789LL);
    ASSERT_TRUE(statement.last_int64_bind.has_value());
    EXPECT_EQ(statement.last_int64_bind->index, 3);
    EXPECT_EQ(statement.last_int64_bind->value, 123456789LL);

    statement.col_i64[7] = -42LL;
    const auto result =
        orm::binder<FakeStatement, std::int64_t>::read(statement, 7);
    EXPECT_EQ(result, -42LL);
}

TEST(OrmBinder, IntBindCastsToInt64AndReadCastsBack)
{
    FakeStatement statement;

    orm::binder<FakeStatement, int>::bind(statement, 1, 42);
    ASSERT_TRUE(statement.last_int64_bind.has_value());
    EXPECT_EQ(statement.last_int64_bind->index, 1);
    EXPECT_EQ(statement.last_int64_bind->value, static_cast<std::int64_t>(42));

    statement.col_i64[0] = 99;
    const auto result    = orm::binder<FakeStatement, int>::read(statement, 0);
    EXPECT_EQ(result, 99);
}

TEST(OrmBinder, BoolBindAs0or1AndReadNonZeroAsTrue)
{
    FakeStatement statement;

    orm::binder<FakeStatement, bool>::bind(statement, 2, true);
    ASSERT_TRUE(statement.last_int64_bind.has_value());
    EXPECT_EQ(statement.last_int64_bind->index, 2);
    EXPECT_EQ(statement.last_int64_bind->value, 1);

    orm::binder<FakeStatement, bool>::bind(statement, 2, false);
    ASSERT_TRUE(statement.last_int64_bind.has_value());
    EXPECT_EQ(statement.last_int64_bind->value, 0);

    statement.col_i64[5] = 0;
    const auto false_result =
        orm::binder<FakeStatement, bool>::read(statement, 5);
    EXPECT_FALSE(false_result);

    statement.col_i64[6] = 2;
    const auto true_result =
        orm::binder<FakeStatement, bool>::read(statement, 6);
    EXPECT_TRUE(true_result);
}

TEST(OrmBinder, DoubleBindAndRead)
{
    FakeStatement statement;

    orm::binder<FakeStatement, double>::bind(statement, 4, 3.14159);
    ASSERT_TRUE(statement.last_double_bind.has_value());
    EXPECT_EQ(statement.last_double_bind->index, 4);
    EXPECT_DOUBLE_EQ(statement.last_double_bind->value, 3.14159);

    statement.col_dbl[9] = -0.25;
    const auto result = orm::binder<FakeStatement, double>::read(statement, 9);
    EXPECT_DOUBLE_EQ(result, -0.25);
}

TEST(OrmBinder, StringBindAndRead)
{
    FakeStatement statement;

    orm::binder<FakeStatement, std::string>::bind(
        statement,
        8,
        std::string{"hello"}
    );
    ASSERT_TRUE(statement.last_text_bind.has_value());
    EXPECT_EQ(statement.last_text_bind->index, 8);
    EXPECT_EQ(statement.last_text_bind->value, "hello");

    statement.col_txt[1] = "world";
    const auto result =
        orm::binder<FakeStatement, std::string>::read(statement, 1);
    EXPECT_EQ(result, "world");
}

TEST(OrmBinder, StrongIdBindUsesValueAndReadUsesFrom)
{
    FakeStatement statement;

    // If this test doesn't compile, replace TestId with your project StrongId
    // type that satisfies `orm::strong_id`.
    orm::binder<FakeStatement, TestId>::bind(statement, 0, TestId::from(777));
    ASSERT_TRUE(statement.last_int64_bind.has_value());
    EXPECT_EQ(statement.last_int64_bind->index, 0);
    EXPECT_EQ(statement.last_int64_bind->value, 777);

    statement.col_i64[3] = 1234;
    const auto result = orm::binder<FakeStatement, TestId>::read(statement, 3);
    EXPECT_EQ(result, TestId::from(1234));
}

TEST(OrmBinder, EnumMetaBindStoresNameAndReadParsesString)
{
    FakeStatement statement;

    orm::binder<FakeStatement, TestEnum>::bind(statement, 5, TestEnum::Beta);
    ASSERT_TRUE(statement.last_text_bind.has_value());
    EXPECT_EQ(statement.last_text_bind->index, 5);
    EXPECT_EQ(statement.last_text_bind->value, "Beta");

    statement.col_txt[2] = "Alpha";
    const auto result =
        orm::binder<FakeStatement, TestEnum>::read(statement, 2);
    EXPECT_EQ(result, TestEnum::Alpha);
}

TEST(OrmBinder, EnumMetaReadInvalidThrowsOrmError)
{
    FakeStatement statement;
    statement.col_txt[0] = "NotAValue";

    try
    {
        (void) orm::binder<FakeStatement, TestEnum>::read(statement, 0);
        FAIL() << "Expected ORMError";
    }
    catch (orm::ORMError const& e)
    {
        // Your ORMError likely stores the message in what()
        const std::string msg = e.what();
        EXPECT_NE(
            msg.find("Invalid currency value in database"),
            std::string::npos
        );
        EXPECT_NE(msg.find("NotAValue"), std::string::npos);
    }
}