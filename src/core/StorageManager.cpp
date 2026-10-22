#include "core/StorageManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace core {
    bool StorageManager::saveInventory(const Inventory &inventory, const std::string &filename) {
        json jArray = json::array();

        for (const auto &item: inventory.getItems()) {
            json itemObj;
            itemObj["type"] = (item->getUnit() == "g") ? "Solid" : "Liquid";
            itemObj["name"] = item->getName();
            itemObj["amount"] = item->getAmount();
            itemObj["expirationDate"] = item->getExpirationDate();

            jArray.push_back(itemObj);
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << jArray.dump(4);
        file.close();
        return true;
    }

    bool StorageManager::loadInventory(Inventory &inventory, const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }


        json jArray;
        try {
            file >> jArray;
        } catch (...) {
            file.close();
            return false;
        }

        file.close();
        inventory.clear();

        for (const auto &itemObj: jArray) {
            std::string type = itemObj["type"];
            std::string name = itemObj["name"];
            double amount = itemObj["amount"];
            std::string expirationDate = itemObj["expirationDate"];

            if (type == "Solid") {
                inventory.addIngredient(std::make_unique<SolidIngredient>(name, amount, expirationDate));
            } else if (type == "Liquid") {
                inventory.addIngredient(std::make_unique<LiquidIngredient>(name, amount, expirationDate));
            }
        }
        return true;
    }
}
