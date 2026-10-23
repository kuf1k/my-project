#include "core/FileExporter.h"
#include <fstream>

namespace core {
    FileExporter::FileExporter(std::string title, std::string filePath)
        : InventoryExporter(std::move(title)), filePath(std::move(filePath)) {
    }

    bool FileExporter::exportData(const Inventory &inventory, const std::string &currentDate) {
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file for export: " << filePath << '\n';
            return false;
        }

        outFile << "========================================\n";
        outFile << " " << reportTitle << "\n";
        outFile << " Date of Report: " << currentDate << "\n";
        outFile << " Total Items: " << inventory.getItemsCount() << "\n";
        outFile << "========================================\n";


        outFile << "=== Inventory Details ===\n";
        for (const auto &item: inventory.getItems()) {
            FreshnessStatus status = item->getFreshnessStatus(currentDate);
            outFile << "- " << item->getName()
                    << " | Amount: " << item->getAmount() << " " << item->getUnit()
                    << " | Exp: " << item->getExpirationDate()
                    << " | Status: " << DateUtils::getStatusLabel(status) << "\n";
        }

        outFile.close();
        return true;
    }

    std::string FileExporter::getFilePath() const {
        return filePath;
    }

    bool FileExporter::exportShoppingListToTxt(const ShoppingList &shoppingList, const std::string &filename) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file for shopping list export: " << filename << '\n';
            return false;
        }

        outFile << "========================================\n";
        outFile << "          NUTRIMESH SHOPPING LIST       \n";
        outFile << "========================================\n\n";

        const auto &items = shoppingList.getItems();
        if (items.empty()) {
            outFile << "Your shopping list is empty!\n";
        } else {
            for (const auto &item : items) {
                outFile << "[ ] " << item.name << " — "
                        << item.amountToBuy << " " << item.unit << "\n";
            }
        }

        outFile << "\n========================================\n";
        outFile.close();
        return true;
    }

}
