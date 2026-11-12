#include <gtest/gtest.h>
#include <filesystem>
#include "core/MealPlanner.h"
#include "core/RecipeRepository.h"
#include "core/Inventory.h"
#include "core/SolidIngredient.h"
#include "core/ShoppingList.h"
#include "core/StorageManager.h"

TEST(MealPlannerTest, AddAndSortMealsByDayAndTime) {
    core::MealPlanner planner;

    planner.addMeal("Saturday", "10:00", "Scrambled Eggs");
    planner.addMeal("Monday", "19:30", "Pasta");
    planner.addMeal("Monday", "08:00", "Oatmeal");

    ASSERT_EQ(planner.getEntriesCount(), 3);
    const auto &entries = planner.getEntries();

    EXPECT_EQ(entries[0].dayOfWeek, "Monday");
    EXPECT_EQ(entries[0].time, "08:00");
    EXPECT_EQ(entries[0].recipeTitle, "Oatmeal");

    EXPECT_EQ(entries[1].dayOfWeek, "Monday");
    EXPECT_EQ(entries[1].time, "19:30");
    EXPECT_EQ(entries[1].recipeTitle, "Pasta");

    EXPECT_EQ(entries[2].dayOfWeek, "Saturday");
    EXPECT_EQ(entries[2].time, "10:00");
    EXPECT_EQ(entries[2].recipeTitle, "Scrambled Eggs");
}

TEST(MealPlannerTest, RemoveMealAtIndexAndClear) {
    core::MealPlanner planner;
    planner.addMeal("Tuesday", "12:00", "Soup");
    planner.addMeal("Friday", "20:00", "Pizza");

    EXPECT_FALSE(planner.removeMealAt(5));
    EXPECT_EQ(planner.getEntriesCount(), 2);

    EXPECT_TRUE(planner.removeMealAt(0));
    EXPECT_EQ(planner.getEntriesCount(), 1);
    EXPECT_EQ(planner.getEntries()[0].recipeTitle, "Pizza");

    planner.clear();
    EXPECT_EQ(planner.getEntriesCount(), 0);
}

TEST(MealPlannerTest, WeeklyCaloriesAndShoppingListIntegration) {
    core::RecipeRepository repo;
    core::Recipe pancakes("Pancakes", 15, 350.0, "Mix and fry");
    pancakes.addRequirement("Flour", 200.0, "g");
    pancakes.addRequirement("Milk", 300.0, "ml");
    repo.addRecipe(pancakes);

    core::MealPlanner planner;
    planner.addMeal("Saturday", "10:00", "Pancakes");
    planner.addMeal("Sunday", "10:00", "Pancakes");

    EXPECT_DOUBLE_EQ(planner.getTotalCalories(repo), 700.0);

    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 250.0, "2026-12-31"));

    core::ShoppingList shoppingList;
    shoppingList.generateForRecipes(planner.getPlannedRecipes(repo), inventory);

    const auto &items = shoppingList.getItems();
    ASSERT_EQ(items.size(), 2);
}

TEST(MealPlannerTest, SaveAndLoadJsonPersistence) {
    const std::string testFile = "test_meal_plan.json";
    std::filesystem::remove(testFile);

    core::MealPlanner planner;
    planner.addMeal("Saturday", "10:00", "Omelette");
    planner.addMeal("Wednesday", "14:00", "Salad");

    EXPECT_TRUE(core::StorageManager::saveMealPlan(planner, testFile));

    core::MealPlanner loadedPlanner;
    EXPECT_TRUE(core::StorageManager::loadMealPlan(loadedPlanner, testFile));

    ASSERT_EQ(loadedPlanner.getEntriesCount(), 2);
    EXPECT_EQ(loadedPlanner.getEntries()[0].dayOfWeek, "Wednesday");
    EXPECT_EQ(loadedPlanner.getEntries()[0].time, "14:00");
    EXPECT_EQ(loadedPlanner.getEntries()[1].dayOfWeek, "Saturday");
    EXPECT_EQ(loadedPlanner.getEntries()[1].time, "10:00");

    std::filesystem::remove(testFile);
}