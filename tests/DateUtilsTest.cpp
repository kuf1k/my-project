#include <gtest/gtest.h>
#include "core/DateUtils.h"

TEST(DateUtilsTest, ValidatesDateFormatCorrectly) {
    EXPECT_TRUE(core::DateUtils::isValidDate("2026-09-30"));
    EXPECT_TRUE(core::DateUtils::isValidDate("2026-12-31"));

    EXPECT_FALSE(core::DateUtils::isValidDate("2026/09/30"));
    EXPECT_FALSE(core::DateUtils::isValidDate("30-09-2026"));
    EXPECT_FALSE(core::DateUtils::isValidDate("invalid-date"));
    EXPECT_FALSE(core::DateUtils::isValidDate("2026-13-01"));
}

TEST (DateUtilsTest, CalculatesDaysUntilCorrectly) {
    EXPECT_EQ(core::DateUtils::getDaysUntil("2026-10-05" ,"2026-10-01"), 4);
    EXPECT_EQ(core::DateUtils::getDaysUntil("2026-10-01", "2026-10-01"), 0);
    EXPECT_EQ(core::DateUtils::getDaysUntil("2026-09-30", "2026-10-01"), -1);
}

TEST(DateUtilsTest, CalculateFreshnessStatus) {
    std::string currentDate = "2026-10-01";
    EXPECT_EQ(core::DateUtils::calculateStatus("2026-10-10", currentDate), core::FreshnessStatus::Fresh);
    EXPECT_EQ(core::DateUtils::calculateStatus("2026-10-01", currentDate), core::FreshnessStatus::ExpiringSoon);
    EXPECT_EQ(core::DateUtils::calculateStatus("2026-09-25", currentDate), core::FreshnessStatus::Expired);
}