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

TEST(InventoryTest, RemoveExpiredDeletesOnlyExpiredItems) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Fresh Flour", 500.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Old Milk", 1000.0, "2026-09-01"));

    std::string currentDate = "2026-10-01";

    size_t removed = inventory.removeExpired(currentDate);

    EXPECT_EQ(removed, 1);
    EXPECT_EQ(inventory.getItemsCount(), 1);
}

TEST(InventoryTest, FindByNameReturnsMatchingItems) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Wheat Flour", 500.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Whole Milk", 1000.0, "2026-10-15"));

    testing::internal::CaptureStdout();
    inventory.findByName("Flour", "2026-10-01");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Wheat Flour"), std::string::npos);
    EXPECT_EQ(output.find("Whole Milk"), std::string::npos);
}

TEST(InventoryTest, PrintExpiringSoonShowsOnlyExpiringItems) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Expiring Milk", 1000.0, "2026-10-01"));
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Fresh Flour", 500.0, "2026-12-31"));

    testing::internal::CaptureStdout();
    inventory.printExpiringSoon("2026-10-01");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Expiring Milk"), std::string::npos);
    EXPECT_EQ(output.find("Fresh Flour"), std::string::npos);
}

TEST(InventoryTest, PrintStatisticsCalculatesCorrectly) {
    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 500.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Sugar", 200.0, "2026-12-31"));
    inventory.addIngredient(std::make_unique<core::LiquidIngredient>("Milk", 1000.0, "2026-10-15"));

    testing::internal::CaptureStdout();
    inventory.printStatistics();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Total Ingredients: 3"), std::string::npos);
    EXPECT_NE(output.find("Solid Ingredients: 2 (Total Weight: 700 g)"), std::string::npos);
    EXPECT_NE(output.find("Liquid Ingredients: 1 (Total Volume: 1000 ml)"), std::string::npos);
}
