#include <gtest/gtest.h>
#include "core/ShoppingList.h"
#include "core/SolidIngredient.h"

TEST(ShoppingListTest, GeneratesCorrectListForMissingIngredients) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 100.0, "2026-12-31"));

    core::Recipe cake("Cake");
    cake.addRequirement("Flour", 500.0, "g");
    cake.addRequirement("Sugar", 200.0, "g");

    core::ShoppingList shopList;
    shopList.generateForRecipe(cake, inventory);

    const auto &items = shopList.getItems();


    ASSERT_EQ(items.size(), 2);


    EXPECT_EQ(items[0].name, "Flour");
    EXPECT_DOUBLE_EQ(items[0].amountToBuy, 400.0);

    EXPECT_EQ(items[1].name, "Sugar");
    EXPECT_DOUBLE_EQ(items[1].amountToBuy, 200.0);
}
