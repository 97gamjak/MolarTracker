// test_validators.cpp
//
// GoogleTest-based tests for ui::LineValidator, ui::EmailLineEdit,
// and ui::NameLineEdit.
//
// Coverage:
//  - empty optional field is valid
//  - empty required field is invalid with correct error text
//  - external error overrides local validity
//  - clearing external error restores local state
//  - validityChanged signal fires on transitions
//  - EmailLineEdit accepts well-formed addresses, rejects malformed ones
//  - NameLineEdit allows alphanumeric/underscore/hyphen/period by default
//  - NameLineEdit allows whitespace after allowWhiteSpaces(true)
//  - NameLineEdit rejects whitespace when allowWhiteSpaces(false)

#include <gtest/gtest.h>

#include <QtTest/QSignalSpy>
#include <QString>

#include "ui/validators/email_line_edit.hpp"
#include "ui/validators/name_line_edit.hpp"

namespace
{

    // --------------- EmailLineEdit ----------------------------------------

    class EmailLineEditTest : public ::testing::Test
    {
       protected:
        ui::EmailLineEdit _edit{nullptr};
    };

    TEST_F(EmailLineEditTest, EmptyOptionalIsValid)
    {
        _edit.setRequired(false);
        _edit.clear();
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, EmptyRequiredIsInvalid)
    {
        _edit.setRequired(true);
        _edit.clear();
        EXPECT_FALSE(_edit.isValid());
        EXPECT_FALSE(_edit.getErrorText().isEmpty());
    }

    TEST_F(EmailLineEditTest, ValidEmailIsAccepted)
    {
        _edit.setRequired(false);
        _edit.setText("user@example.com");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, ValidEmailWithSubdomainIsAccepted)
    {
        _edit.setRequired(false);
        _edit.setText("user.name+tag@sub.domain.org");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingAtSignIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("notanemail");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingDomainIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("user@");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, MissingTldIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("user@domain");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, LeadingAtIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("@nodomain.com");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, ExternalErrorMakesValidInputInvalid)
    {
        _edit.setRequired(false);
        _edit.setText("user@example.com");
        ASSERT_TRUE(_edit.isValid());

        _edit.setExternalError("Email already taken");
        EXPECT_FALSE(_edit.isValid());
        EXPECT_EQ(_edit.getErrorText(), "Email already taken");
    }

    TEST_F(EmailLineEditTest, ClearingExternalErrorRestoresValidity)
    {
        _edit.setRequired(false);
        _edit.setText("user@example.com");
        _edit.setExternalError("Error");
        _edit.clearExternalError();
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(EmailLineEditTest, ValidityChangedSignalFires)
    {
        _edit.setRequired(true);
        QSignalSpy spy(&_edit, &ui::LineValidator::validityChanged);

        _edit.setText("user@example.com");
        ASSERT_GE(spy.count(), 1);
        EXPECT_TRUE(spy.last().at(0).toBool());
    }

    TEST_F(EmailLineEditTest, WhitespaceOnlyTreatedAsEmpty)
    {
        _edit.setRequired(true);
        _edit.setText("   ");
        EXPECT_FALSE(_edit.isValid());
    }

    // --------------- NameLineEdit -----------------------------------------

    class NameLineEditTest : public ::testing::Test
    {
       protected:
        ui::NameLineEdit _edit{nullptr};
    };

    TEST_F(NameLineEditTest, EmptyOptionalIsValid)
    {
        _edit.setRequired(false);
        _edit.clear();
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, EmptyRequiredIsInvalid)
    {
        _edit.setRequired(true);
        _edit.clear();
        EXPECT_FALSE(_edit.isValid());
        EXPECT_FALSE(_edit.getErrorText().isEmpty());
    }

    TEST_F(NameLineEditTest, AlphanumericIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("ValidName123");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, UnderscoreIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("valid_name");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, HyphenIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("valid-name");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, PeriodIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("valid.name");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, AtSignIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("bad@name");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, ExclamationMarkIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("bad!name");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, SpaceRejectedByDefault)
    {
        _edit.setRequired(false);
        _edit.allowWhiteSpaces(false);
        _edit.setText("name with space");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, SpaceAllowedAfterEnablingWhiteSpaces)
    {
        _edit.setRequired(false);
        _edit.allowWhiteSpaces(true);
        _edit.setText("name with space");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, DisablingWhiteSpacesRejectsSpace)
    {
        _edit.setRequired(false);
        _edit.allowWhiteSpaces(true);
        _edit.setText("ok name");
        ASSERT_TRUE(_edit.isValid());

        _edit.allowWhiteSpaces(false);
        _edit.setText("bad name");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(NameLineEditTest, ExternalErrorOverridesValidInput)
    {
        _edit.setRequired(false);
        _edit.setText("GoodName");
        ASSERT_TRUE(_edit.isValid());

        _edit.setExternalError("Name already exists");
        EXPECT_FALSE(_edit.isValid());
        EXPECT_EQ(_edit.getErrorText(), "Name already exists");
    }

    TEST_F(NameLineEditTest, TextChangeClears_ExternalError)
    {
        _edit.setRequired(false);
        _edit.setText("GoodName");
        _edit.setExternalError("Taken");
        ASSERT_FALSE(_edit.isValid());

        _edit.setText("OtherName");
        EXPECT_TRUE(_edit.isValid());
    }

}   // namespace
