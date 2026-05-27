// test_amount_line_edit.cpp
//
// GoogleTest-based tests for ui::AmountLineEdit.
//
// Coverage:
//  - empty optional is valid; empty required is invalid
//  - valid numeric inputs accepted for default 2 decimal places
//  - non-numeric input rejected
//  - getAmount() returns correct micro_units value
//  - setOnlyPositive() rejects negative amounts
//  - setNDecimalPlaces(0) rejects fractional input
//  - setNDecimalPlaces(4) accepts up to 4 decimal places

#include <gtest/gtest.h>

#include <QString>

#include "config/quantity.hpp"
#include "ui/validators/amount_line_edit.hpp"

namespace
{

    class AmountLineEditTest : public ::testing::Test
    {
       protected:
        ui::AmountLineEdit _edit{nullptr};
    };

    TEST_F(AmountLineEditTest, EmptyOptionalIsValid)
    {
        _edit.setRequired(false);
        _edit.clear();
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, EmptyRequiredIsInvalid)
    {
        _edit.setRequired(true);
        _edit.clear();
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, IntegerInputIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("42");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, ZeroIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("0");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, TwoDecimalPlacesIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("12.34");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, OneDecimalPlaceIsValid)
    {
        _edit.setRequired(false);
        _edit.setText("5.5");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, AlphaInputIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("abc");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, MixedAlphaNumericIsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("12abc");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, MultipleDotsRejected)
    {
        _edit.setRequired(false);
        _edit.setText("12.34.56");
        EXPECT_FALSE(_edit.isValid());
    }

    // --------------- getAmount() ------------------------------------------

    TEST_F(AmountLineEditTest, GetAmountEmptyReturnsZero)
    {
        _edit.clear();
        EXPECT_EQ(_edit.getAmount(), 0);
    }

    TEST_F(AmountLineEditTest, GetAmountInteger)
    {
        _edit.setText("10");
        EXPECT_EQ(_edit.getAmount(), 1000);   // 10 * 100 (2 decimal places)
    }

    TEST_F(AmountLineEditTest, GetAmountWithDecimals)
    {
        _edit.setText("1.50");
        EXPECT_EQ(_edit.getAmount(), 150);
    }

    TEST_F(AmountLineEditTest, GetAmountFractionalOnly)
    {
        _edit.setText("0.01");
        EXPECT_EQ(_edit.getAmount(), 1);
    }

    // --------------- setOnlyPositive() ------------------------------------

    TEST_F(AmountLineEditTest, PositiveInputAllowedByDefault)
    {
        _edit.setRequired(false);
        _edit.setOnlyPositive(false);
        _edit.setText("100");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, OnlyPositiveRejectsNegativeSign)
    {
        _edit.setRequired(false);
        _edit.setOnlyPositive(true);
        _edit.setText("-5");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, OnlyPositiveAcceptsPositive)
    {
        _edit.setRequired(false);
        _edit.setOnlyPositive(true);
        _edit.setText("99.99");
        EXPECT_TRUE(_edit.isValid());
    }

    // --------------- setNDecimalPlaces() ----------------------------------

    TEST_F(AmountLineEditTest, ZeroDecimalPlacesRejectsFraction)
    {
        _edit.setRequired(false);
        _edit.setNDecimalPlaces(0);
        _edit.setText("12.3");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, ZeroDecimalPlacesAcceptsInteger)
    {
        _edit.setRequired(false);
        _edit.setNDecimalPlaces(0);
        _edit.setText("42");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, FourDecimalPlacesAcceptsFourDecimals)
    {
        _edit.setRequired(false);
        _edit.setNDecimalPlaces(4);
        _edit.setText("1.2345");
        EXPECT_TRUE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, TwoDecimalPlacesRejectsThreeDecimals)
    {
        _edit.setRequired(false);
        _edit.setNDecimalPlaces(2);
        _edit.setText("1.234");
        EXPECT_FALSE(_edit.isValid());
    }

    TEST_F(AmountLineEditTest, GetAmountWithFourDecimalPlaces)
    {
        _edit.setNDecimalPlaces(4);
        _edit.setText("1.0001");
        EXPECT_EQ(_edit.getAmount(), 10001);
    }

}   // namespace
