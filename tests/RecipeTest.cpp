#include <gtest/gtest.h>
#include "core/Recipe.h"

TEST(RecipeTest, RecipeCreationAndTitle) {
    core::Recipe recipe("Pancakes");
    EXPECT_EQ(recipe.getTitle(), "Pancakes");
}

TEST(RecipeTest, AddRequirements) {
    core::Recipe recipe("Pancakes");

    recipe.addRequirement("Flour", 200.0, "g");
    recipe.addRequirement("Milk", 500.0, "ml");

    const auto &reqs = recipe.getRequirements();

    ASSERT_EQ(reqs.size(), 2);

    EXPECT_EQ(reqs[0].name, "Flour");
    EXPECT_DOUBLE_EQ(reqs[0].requiredAmount, 200.0);
    EXPECT_EQ(reqs[0].unit, "g");

    EXPECT_EQ(reqs[1].name, "Milk");
    EXPECT_DOUBLE_EQ(reqs[1].requiredAmount, 500.0);
    EXPECT_EQ(reqs[1].unit, "ml");
}
