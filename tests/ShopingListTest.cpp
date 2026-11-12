#include <gtest/gtest.h>
#include "core/ShoppingList.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

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

TEST(ShoppingListTest, IgnoresInventoryItemWithDifferentUnit) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Eggs", 500.0, "2026-12-31"));

    core::Recipe omelette("Omelette");
    omelette.addRequirement("Eggs", 3.0, "pcs");

    core::ShoppingList shoppingList;
    shoppingList.generateForRecipe(omelette, inventory);

    ASSERT_EQ(shoppingList.getItems().size(), 1);
    EXPECT_EQ(shoppingList.getItems()[0].name, "Eggs");
    EXPECT_DOUBLE_EQ(shoppingList.getItems()[0].amountToBuy, 3.0);
    EXPECT_EQ(shoppingList.getItems()[0].unit, "pcs");
}
