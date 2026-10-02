#include <gtest/gtest.h>
#include "core/DataFilter.h"
#include "core/Recipe.h"

TEST(DataFilterTest, FilterRecipesByCalories) {
    std::vector<core::Recipe> recipes;
    recipes.emplace_back("Light Salad", 10, 150.0, "Mix greens.");
    recipes.emplace_back("Pizza", 30, 800.0, "Bake dough with cheese.");
    recipes.emplace_back("Smoothie", 5, 200.0, "Blend fruits.");

    auto lowCal = core::DataFilter<core::Recipe>::filterBy(recipes, [](const core::Recipe &recipe) {
        return recipe.getCalories() < 300.0;
    });

    ASSERT_EQ(lowCal.size(), 2);
    EXPECT_EQ(lowCal[0].getTitle(), "Light Salad");
    EXPECT_EQ(lowCal[1].getTitle(), "Smoothie");
}

TEST(DataFilterTest, ContainsCheck) {
    std::vector<int> numbers = {10, 20, 30, 40, 50};

    bool hasLargeNumber = core::DataFilter<int>::contains(numbers, [](int n) {
        return n > 35;
    });
    EXPECT_TRUE(hasLargeNumber);
}
