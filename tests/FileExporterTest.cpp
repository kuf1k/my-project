#include <gtest/gtest.h>
#include "core/FileExporter.h"
#include "core/SolidIngredient.h"
#include "core/CountableIngredient.h"
#include <fstream>


TEST(FileExporterTest, ExportDataCreatesFileAndWritesHeader) {
    std::string testFileName = "test_export_output.txt";


    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::SolidIngredient>("Flour", 500.0, "2026-12-31"));


    std::unique_ptr<core::InventoryExporter> exporter =
            std::make_unique<core::FileExporter>("Test Status Report", testFileName);

    EXPECT_EQ(exporter->getReportTitle(), "Test Status Report");


    bool success = exporter->exportData(inventory, "2026-10-02");
    EXPECT_TRUE(success);


    std::ifstream inFile(testFileName);
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    bool foundTitle = false;
    while (std::getline(inFile, line)) {
        if (line.find("Test Status Report") != std::string::npos) {
            foundTitle = true;
            break;
        }
    }
    inFile.close();

    EXPECT_TRUE(foundTitle);


    std::remove(testFileName.c_str());
}

TEST(FileExporterTest, ExportDataWritesCountableIngredientsCorrectly) {
    std::string testFileName = "test_countable_export.txt";

    core::Inventory inventory;
    inventory.addIngredient(std::make_unique<core::CountableIngredient>("Eggs", 12.0, "2026-11-15"));

    core::FileExporter exporter("Inventory Report", testFileName);
    bool success = exporter.exportData(inventory, "2026-10-22");
    EXPECT_TRUE(success);

    std::ifstream inFile(testFileName);
    ASSERT_TRUE(inFile.is_open());

    std::string content((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());
    inFile.close();

    EXPECT_NE(content.find("Eggs"), std::string::npos);
    EXPECT_NE(content.find("pcs"), std::string::npos);

    std::remove(testFileName.c_str());
}

TEST(FileExporterTest, ExportShoppingListToTxt) {
    std::string testFileName = "test_shopping_list_export.txt";

    core::Inventory emptyInventory;
    core::Recipe recipe("Pancakes", 20, 300, "Mix and cook");
    recipe.addRequirement("Milk", 100.0, "ml");
    recipe.addRequirement("Eggs", 2.0, "pcs");

    core::ShoppingList list;
    list.generateForRecipe(recipe, emptyInventory);

    bool success = core::FileExporter::exportShoppingListToTxt(list, testFileName);
    EXPECT_TRUE(success);

    std::ifstream inFile(testFileName);
    ASSERT_TRUE(inFile.is_open());

    std::string content((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());
    inFile.close();

    EXPECT_NE(content.find("NUTRIMESH SHOPPING LIST"), std::string::npos);
    EXPECT_NE(content.find("Milk — 100 ml"), std::string::npos);
    EXPECT_NE(content.find("Eggs — 2 pcs"), std::string::npos);

    std::remove(testFileName.c_str());
}
