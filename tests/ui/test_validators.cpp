#include <gtest/gtest.h>

#include <QApplication>

#include "ui/validators/amount_line_edit.hpp"
#include "ui/validators/email_line_edit.hpp"
#include "ui/validators/name_line_edit.hpp"

namespace
{
    // -------------------------------------------------------------------------
    // NameLineEdit
    // -------------------------------------------------------------------------

    class NameLineEditTest : public ::testing::Test
    {
       protected:
        ui::NameLineEdit _edit{nullptr};
    };

    TEST_F(NameLineEditTest, NotRequiredEmptyIsValid)
    {
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, RequiredEmptyIsInvalid)
    {
        _edit.setRequired(true);
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, RequiredErrorMessageMentionsName)
    {
        _edit.setRequired(true);
        EXPECT_TRUE(_edit.getErrorText().contains("name", Qt::CaseInsensitive));
    }

    TEST_F(NameLineEditTest, AlphanumericAndUnderscoreAreValid)
    {
        _edit.setRequired(true);
        _edit.setText("JohnDoe_123");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, HyphenAndPeriodAreValid)
    {
        _edit.setRequired(true);
        _edit.setText("John-Doe.Jr");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, ExclamationMarkIsInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("John!");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, WhitespacesDisallowedByDefault)
    {
        _edit.setRequired(true);
        _edit.setText("John Doe");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, WhitespacesAllowedWhenEnabled)
    {
        _edit.setRequired(true);
        _edit.allowWhiteSpaces(true);
        _edit.setText("John Doe");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, ExternalErrorMakesInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("ValidName");
        ASSERT_TRUE(_edit.isValid());

        _edit.setExternalError("Name already taken");
        EXPECT_FALSE(_edit.isValid());
        EXPECT_EQ(_edit.getErrorText(), "Name already taken");
    }

    TEST_F(NameLineEditTest, ClearExternalErrorRestoresValidity)
    {
        _edit.setRequired(true);
        _edit.setText("ValidName");
        _edit.setExternalError("Name already taken");
        ASSERT_FALSE(_edit.isValid());

        _edit.clearExternalError();
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, TextChangeAfterExternalErrorClearsIt)
    {
        _edit.setRequired(true);
        _edit.setText("ValidName");
        _edit.setExternalError("Name already taken");
        ASSERT_FALSE(_edit.isValid());

        _edit.setText("AnotherName");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, EmptyValidityChangesWhenRequiredToggled)
    {
        EXPECT_TRUE(_edit.isValid());
        _edit.setRequired(true);
        EXPECT_FALSE(_edit.isValid());
        _edit.setRequired(false);
        EXPECT_TRUE(_edit.isValid());
    }

    // -------------------------------------------------------------------------
    // EmailLineEdit
    // -------------------------------------------------------------------------

    class EmailLineEditTest : public ::testing::Test
    {
       protected:
        ui::EmailLineEdit _edit{nullptr};
    };

    TEST_F(EmailLineEditTest, NotRequiredEmptyIsValid)
    {
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, RequiredEmptyIsInvalid)
    {
        _edit.setRequired(true);
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, RequiredErrorMessageMentionsEmail)
    {
        _edit.setRequired(true);
        EXPECT_TRUE(_edit.getErrorText().contains("email", Qt::CaseInsensitive)
        );
    }

    TEST_F(EmailLineEditTest, ValidEmailIsValid)
    {
        _edit.setRequired(true);
        _edit.setText("test@example.com");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, EmailWithSubdomainIsValid)
    {
        _edit.setRequired(true);
        _edit.setText("user@mail.example.org");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingAtSignIsInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("testexample.com");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingDomainIsInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("test@");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingTopLevelDomainIsInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("test@example");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, ExternalErrorMakesInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("test@example.com");
        ASSERT_TRUE(_edit.isValid());

        _edit.setExternalError("Email already registered");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, ClearExternalErrorRestoresValidity)
    {
        _edit.setRequired(true);
        _edit.setText("test@example.com");
        _edit.setExternalError("Email already registered");
        ASSERT_FALSE(_edit.isValid());

        _edit.clearExternalError();
        EXPECT_TRUE(_edit.isValid());
    }

    // -------------------------------------------------------------------------
    // AmountLineEdit
    // -------------------------------------------------------------------------

    class AmountLineEditTest : public ::testing::Test
    {
       protected:
        ui::AmountLineEdit _edit{nullptr};
    };

    TEST_F(AmountLineEditTest, NotRequiredEmptyIsValid)
    {
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, RequiredEmptyIsInvalid)
    {
        _edit.setRequired(true);
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, RequiredErrorMessageMentionsAmount)
    {
        _edit.setRequired(true);
        EXPECT_TRUE(_edit.getErrorText().contains("amount", Qt::CaseInsensitive)
        );
    }

    TEST_F(AmountLineEditTest, ValidIntegerIsValid)
    {
        _edit.setRequired(true);
        _edit.setText("100");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, ValidDecimalWithTwoPlacesIsValid)
    {
        _edit.setRequired(true);
        _edit.setText("100.50");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, ZeroIsValid)
    {
        _edit.setRequired(true);
        _edit.setText("0");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, TooManyDecimalsIsInvalid)
    {
        _edit.setRequired(true);
        _edit.setNDecimalPlaces(2);
        _edit.setText("100.123");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, AlphaCharactersAreInvalid)
    {
        _edit.setRequired(true);
        _edit.setText("abc");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, NegativeAmountIsInvalidByRegex)
    {
        _edit.setRequired(true);
        _edit.setText("-100");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, IsZeroTrueForEmpty)
    {
        EXPECT_TRUE(_edit.isZero());
    }

    TEST_F(AmountLineEditTest, IsZeroTrueForZeroText)
    {
        _edit.setText("0");
        EXPECT_TRUE(_edit.isZero());
    }

    TEST_F(AmountLineEditTest, IsZeroFalseForNonZero)
    {
        _edit.setText("100");
        EXPECT_FALSE(_edit.isZero());
    }

    TEST_F(AmountLineEditTest, GetAmountConvertsInteger)
    {
        _edit.setText("100");
        // 100 with precision 2 → 100 * 100 = 10000
        EXPECT_EQ(_edit.getAmount(2), 10000);
    }

    TEST_F(AmountLineEditTest, GetAmountConvertsDecimal)
    {
        _edit.setText("100.50");
        // 100.50 with precision 2 → 10050
        EXPECT_EQ(_edit.getAmount(2), 10050);
    }

    TEST_F(AmountLineEditTest, GetAmountReturnsZeroForEmpty)
    {
        EXPECT_EQ(_edit.getAmount(2), 0);
    }

    TEST_F(AmountLineEditTest, ZeroDecimalPlacesRejectsDecimalInput)
    {
        _edit.setRequired(true);
        _edit.setNDecimalPlaces(0);
        _edit.setText("100.5");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, ZeroDecimalPlacesAcceptsInteger)
    {
        _edit.setRequired(true);
        _edit.setNDecimalPlaces(0);
        _edit.setText("100");
        EXPECT_TRUE(_edit.isValid());
    }

}   // namespace
