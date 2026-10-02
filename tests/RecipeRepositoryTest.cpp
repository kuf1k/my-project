#include <gtest/gtest.h>
#include "core/RecipeRepository.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

TEST(RecipeRepositoryTest, AddAndCountRecipes) {
    core::RecipeRepository repo;
    EXPECT_EQ(repo.getRecipesCount(), 0);

    core::Recipe pancakes("Pancakes", 15, 350.0, "Mix and fry.");
    repo.addRecipe(pancakes);

    EXPECT_EQ(repo.getRecipesCount(), 1);
}

TEST(RecipeRepositoryTest, GetCookableRecipesReturnsOnlyMatching) {
    core::RecipeRepository repo;

    core::Recipe pancakes("Pancakes", 15, 350.0, "Mix and fry!");
    pancakes.addRequirement("Flour", 200.0, "g");
    pancakes.addRequirement("Milk", 500.0, "ml");

    core::Recipe cake("Cake", 45, 600.0, "Bake in oven!");
    cake.addRequirement("Flour", 1000.0, "g");

    repo.addRecipe(pancakes);
    repo.addRecipe(cake);

    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 300.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Milk", 600.0, "2026-12-31"));

    auto cookable = repo.getCookableRecipes(inventory);

    ASSERT_EQ(cookable.size(), 1);
    EXPECT_EQ(cookable[0].getTitle(), "Pancakes");
}