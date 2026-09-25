#include <gtest/gtest.h>
#include "core/Inventory.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

TEST(InventoryTest, InitialStateIsEmpty) {
    core::Inventory inventory;
    EXPECT_EQ(inventory.getItemsCount(), 0);
}

TEST(InventoryTest, AddIngredientIncreasesCount) {
    core::Inventory inventory;
    auto flour = std::make_unique<core::SolidIngredient>("Flour", 500.0, "2026-12-31");
    inventory.addIngredient(std::move(flour));
    auto milk = std::make_unique<core::LiquidIngredient>("Milk", 1000.0, "2026-10-15");
    inventory.addIngredient(std::move(milk));
    EXPECT_GE(inventory.getItemsCount(), 2);
}