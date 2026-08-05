// tests/common/test_quantity.cpp
//
// GoogleTest-based tests for microUnitsFromString / microUnitsToString.
//
// Coverage:
//  - microUnitsToString renders a value stored at `scale` decimal places
//    correctly when asked to display fewer decimal places than `scale`
//    (regression test for MOLTRACK-334: "1.05" rendering as "1.50000"
//    when displayed with precision=2 while stored at scale=6)
//  - precision == scale is a no-op rescale
//  - precision > scale pads with zeros
//  - display truncates rather than rounds, matching
//    microUnitsFromString's truncate policy

#include <gtest/gtest.h>

#include "common/quantity.hpp"

TEST(Quantity, ToStringWithFewerDecimalsThanScaleRendersCorrectly)
{
    // "1.05" stored at scale 6 is 1'050'000 micro_units.
    const auto value = microUnitsFromString("1.05", 6);

    EXPECT_EQ("1.05", microUnitsToString(value, 2, 6));
    EXPECT_EQ("1.050000", microUnitsToString(value, 6, 6));
}

TEST(Quantity, ToStringWithMoreDecimalsThanScalePadsWithZeros)
{
    const auto value = microUnitsFromString("1.05", 2);

    EXPECT_EQ("1.05", microUnitsToString(value, 2, 2));
    EXPECT_EQ("1.050000", microUnitsToString(value, 6, 2));
}

TEST(Quantity, ToStringWithFewerDecimalsTruncatesRatherThanRounds)
{
    const auto value = microUnitsFromString("123.456789", 6);

    EXPECT_EQ("123.45", microUnitsToString(value, 2, 6));
}

TEST(Quantity, ToStringPreservesLeadingZerosInFraction)
{
    const auto value = microUnitsFromString("0.01", 6);

    EXPECT_EQ("0.01", microUnitsToString(value, 2, 6));
}

TEST(Quantity, ToStringHandlesNegativeValues)
{
    const auto value = microUnitsFromString("-1.05", 6);

    EXPECT_EQ("-1.05", microUnitsToString(value, 2, 6));
}

TEST(Quantity, FromStringRoundTripsThroughMultiplePrecisions)
{
    for (const std::uint8_t precision : {0, 1, 2, 6, 8})
    {
        const auto value = microUnitsFromString("42.5", precision);
        const auto text  = microUnitsToString(value, precision, precision);

        EXPECT_EQ(value, microUnitsFromString(text, precision))
            << "round-trip failed for precision "
            << static_cast<int>(precision);
    }
}
