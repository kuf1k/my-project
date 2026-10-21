#include <gtest/gtest.h>
#include "core/StorageManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"

TEST(StorageManagerTest, SaveAndLoadInventorySuccessfully) {
    std::string testFileName = "test_inventory_storage.csv";

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
