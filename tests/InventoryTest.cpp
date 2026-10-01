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
    EXPECT_EQ(inventory.getItemsCount(), 2);
}

TEST(InventroyTest, RemoveExpiredDeletsOnlyExpiredItems) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Fresh Flour", 500.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Old Milk", 1000.0, "2026-09-01"));

    std::string currentDate = "2026-10-01";

    size_t removed = inventory.removeExpired(currentDate);

    EXPECT_EQ(removed,1);
    EXPECT_EQ(inventory.getItemsCount(),1);
}
