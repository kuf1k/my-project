#include "core/StorageManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include <fstream>
#include <sstream>
namespace core {
    bool StorageManager::saveInventory(const Inventory& inventory, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        for (const auto& item : inventory.getItems()) {
            std::string type = (item->getUnit() == "g") ? "Solid" : "Liquid";
            file << type << ","
                 << item->getName() << ","
                 << item->getAmount() << ","
                 << item->getExpirationDate() << "\n";
        }

        file.close();
        return true;
    }

    bool StorageManager::loadInventory(Inventory& inventory, const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        inventory.clear();

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string type, name, amountStr, expirationDate;

            std::getline(ss, type, ',');
            std::getline(ss, name, ',');
            std::getline(ss, amountStr, ',');
            std::getline(ss, expirationDate, ',');

            double amount = std::stod(amountStr);

            if (type == "Solid") {
                inventory.addIngredient(std::make_unique<SolidIngredient>(name, amount, expirationDate));
            } else if (type == "Liquid") {
                inventory.addIngredient(std::make_unique<LiquidIngredient>(name, amount, expirationDate));
            }
        }

        file.close();
        return true;
    }
}