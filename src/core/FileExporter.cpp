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

        outFile.close();
        return true;
    }

    std::string FileExporter::getFilePath() const {
        return filePath;
    }
}
