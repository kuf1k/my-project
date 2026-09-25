#include <gtest/gtest.h>
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

TEST(SolidIngredientTest, InizialiazationAndUnit) {
    core::SolidIngredient flour("Flour", 500.0, "2026-12-31");
    EXPECT_EQ(flour.getName(), "Flour");
    EXPECT_DOUBLE_EQ(flour.getAmount(), 500.0);
    EXPECT_EQ(flour.getExpirationDate(), "2026-12-31");
    EXPECT_EQ(flour.getUnit(), "g");
}

TEST(LiquidIngredientTest, InizialiazationAndUnit) {
    core::LiquidIngredient milk("Milk", 1000.0, "2026-10-15");
    EXPECT_EQ(milk.getName(), "Milk");
    EXPECT_DOUBLE_EQ(milk.getAmount(), 1000.0);
    EXPECT_EQ(milk.getExpirationDate(), "2026-10-15");
    EXPECT_EQ(milk.getUnit(), "ml");
}
