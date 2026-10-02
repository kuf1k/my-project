#include <gtest/gtest.h>
#include "core/Inventory.h"
#include "core/LiquidIngredient.h"
#include "core/Recipe.h"
#include "core/RecipeMatcher.h"
#include "core/SolidIngredient.h"

TEST(RecipeMatcherTest, CanPrepareReturnsTrueWhenEnoughIngredients) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 500.0, "2026-12-31" ""));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Milk", 1000.0, "2026-12-31"));

    core::Recipe pancake("Pancakes");
    pancake.addRequirement("Flour", 200.0, "g");
    pancake.addRequirement("Milk", 500.0, "ml");

    EXPECT_TRUE(core::RecipeMatcher::canPrepare(pancake,inventory));
}

TEST(RecipeMatcherTest, CanPrepareReturnsFalseWhenMissingIngredients) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 100.0, "2026-12-31"));

    core::Recipe pancake("Pancakes");
    pancake.addRequirement("Flour", 200.0, "g");

    EXPECT_FALSE(core::RecipeMatcher::canPrepare(pancake, inventory));
}
