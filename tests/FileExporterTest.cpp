#include <gtest/gtest.h>
#include "core/FileExporter.h"
#include "core/SolidIngredient.h"
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
