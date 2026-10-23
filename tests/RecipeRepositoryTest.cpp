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

TEST(RecipeRepositoryTest, PreventDuplicateRecipes) {
    core::RecipeRepository repo;
    core::Recipe pancakes("Pancakes",15,350.0,"Mix and fry");

    repo.addRecipe(pancakes);
    EXPECT_EQ(repo.getRecipesCount(),1);
    EXPECT_TRUE(repo.hasRecipe("Pancakes"));

    repo.addRecipe(pancakes);
    EXPECT_EQ(repo.getRecipesCount(),1);
}

TEST(RecipeRepositoryTest, GetSortedRecipesByNameAndCalories) {
    core::RecipeRepository repo;

    core::Recipe r1("Pancakes", 20, 400.0, "Mix and fry");
    core::Recipe r2("Apple Pie", 50, 250.0, "Bake in oven");
    core::Recipe r3("Omelette", 10, 150.0, "Whisk and fry");

    repo.addRecipe(r1);
    repo.addRecipe(r2);
    repo.addRecipe(r3);

    auto sortedByName = repo.getSortedRecipes(core::RecipeSortOption::ByNameAsc);
    EXPECT_EQ(sortedByName[0].getTitle(), "Apple Pie");
    EXPECT_EQ(sortedByName[1].getTitle(), "Omelette");
    EXPECT_EQ(sortedByName[2].getTitle(), "Pancakes");

    auto sortedByCalories = repo.getSortedRecipes(core::RecipeSortOption::ByCaloriesAsc);
    EXPECT_EQ(sortedByCalories[0].getTitle(), "Omelette");
    EXPECT_EQ(sortedByCalories[1].getTitle(), "Apple Pie");
    EXPECT_EQ(sortedByCalories[2].getTitle(), "Pancakes");

    std::vector customList = { r1, r2 };
    auto sortedByPrepTime = core::RecipeRepository::sortRecipes(customList, core::RecipeSortOption::ByPrepTimeAsc);

    EXPECT_EQ(sortedByPrepTime[0].getTitle(), "Pancakes");
    EXPECT_EQ(sortedByPrepTime[1].getTitle(), "Apple Pie");
}