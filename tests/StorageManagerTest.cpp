#include <gtest/gtest.h>
#include "core/StorageManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

TEST(StorageManagerTest, SaveAndLoadInventorySuccessfully) {
    std::string testFileName = "test_inventory_storage.json";

    core::Inventory originalInventory;
    originalInventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 400.0, "2026-12-31"));
    originalInventory.addIngredient(std::make_unique<core::LiquidIngredient>("Milk", 750.0, "2026-10-15"));

    bool saveResult = core::StorageManager::saveInventory(originalInventory, testFileName);
    EXPECT_TRUE(saveResult);

    core::Inventory loadedInventory;
    bool loadResult = core::StorageManager::loadInventory(loadedInventory, testFileName);
    EXPECT_TRUE(loadResult);

    EXPECT_EQ(loadedInventory.getItemsCount(), 2);
    EXPECT_DOUBLE_EQ(loadedInventory.getIngredientAmount("Flour"), 400.0);
    EXPECT_DOUBLE_EQ(loadedInventory.getIngredientAmount("Milk"), 750.0);

    std::remove(testFileName.c_str());
}

TEST(StorageManagerTest, SaveAndLoadRecipesSuccessfully) {
    std::string testFileName = "test_recipes_storage.json";

    core::RecipeRepository originalRepo;
    core::Recipe omelette("Omelette", 10, 250.0, "Beat eggs with milk and fry.");
    omelette.addRequirement("Eggs", 3.0, "pcs");
    omelette.addRequirement("Milk", 100.0, "ml");
    originalRepo.addRecipe(omelette);

    bool saveResult = core::StorageManager::saveRecipes(originalRepo, testFileName);
    EXPECT_TRUE(saveResult);

    core::RecipeRepository loadedRepo;
    bool loadResult = core::StorageManager::loadRecipes(loadedRepo, testFileName);
    EXPECT_TRUE(loadResult);

    EXPECT_EQ(loadedRepo.getRecipesCount(), 1);
    EXPECT_TRUE(loadedRepo.hasRecipe("Omelette"));

    const auto &recipes = loadedRepo.getRecipes();
    ASSERT_EQ(recipes.size(), 1);
    EXPECT_EQ(recipes[0].getTitle(), "Omelette");
    EXPECT_EQ(recipes[0].getPrepTime(), 10);
    EXPECT_DOUBLE_EQ(recipes[0].getCalories(), 250.0);
    EXPECT_EQ(recipes[0].getRequirements().size(), 2);

    std::remove(testFileName.c_str());
}
