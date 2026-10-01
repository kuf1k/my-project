#include "core/Inventory.h"
#include "core/DateUtils.h"
#include <iostream>
#include <vector>

namespace core {
    void Inventory::addIngredient(std::unique_ptr<Ingredient> ingredient) {
        if (ingredient) {
            items.push_back(std::move(ingredient));
        }
    }

    void Inventory::printInventory(const std::string &currentDate) const {
        std::cout << "\n=== Your Inventory ===\n";
        if (items.empty()) {
            std::cout << " Your Inventory is empty!\n";
            return;
        }
        for (const auto &item: items) {
            item->printInfo();

            FreshnessStatus status = item->getFreshnessStatus(currentDate);
            std::cout << "Status:" << DateUtils::getStatusLabel(status) << '\n';
        }
    }

    size_t Inventory::getItemsCount() const {
        return items.size();
    }

    size_t Inventory::removeExpired(const std::string &currentDate) {
        size_t removedCount = (std::erase_if(items, [&](const std::unique_ptr<Ingredient> &item) {
            return item->getFreshnessStatus(currentDate) == FreshnessStatus::Expired;
        }));
        return removedCount;
    }

    void Inventory::findByName(const std::string &query, const std::string &currentDate) const {
        std::cout << "\n=== Search Results for '" << query << "' ===\n";
        bool found = false;
        for (const auto &item: items) {
            if (item->getName().find(query) != std::string::npos) {
                item->printInfo();
                FreshnessStatus status = item->getFreshnessStatus(currentDate);
                std::cout << "Status: " << DateUtils::getStatusLabel(status);
                found = true;
            }
        }
        if (!found) {
            std::cout << "No ingredients found matching " << query << "\n";
        }
    }

    void Inventory::printExpiringSoon(const std::string currentDate) const {
        std::cout << "\n=== Ingredients Expiring Soon ===\n";
        bool found = false;

        for (const auto &item: items) {
            if (item->getFreshnessStatus(currentDate) == FreshnessStatus::ExpiringSoon) {
                item->printInfo();
                FreshnessStatus status = item->getFreshnessStatus(currentDate);
                std::cout << "Status: " << DateUtils::getStatusLabel(status);
                found = true;
            }
        }
        if (!found) {
            std::cout << "No ingredients expiring soon!\n";
        }
    }

    void Inventory::printStatistics() const {
        double totalSolidWeight = 0.0;
        double totalLiquidVolume = 0.0;
        size_t solidCount = 0;
        size_t liquidCount = 0;

        for (const auto &item: items) {
            if (item->getUnit() == "g") {
                totalSolidWeight += item->getAmount();
                solidCount++;
            } else if (item->getUnit() == "ml") {
                totalLiquidVolume += item->getAmount();
                liquidCount++;
            }
        }

        std::cout << "\n=================================\n";
        std::cout << "      Inventory Statistics       \n";
        std::cout << "=================================\n";
        std::cout << "Total Ingredients: " << items.size() << '\n';
        std::cout << "Solid Ingredients: " << solidCount << " (Total Weight: " << totalSolidWeight << " g)\n";
        std::cout << "Liquid Ingredients: " << liquidCount << " (Total Volume: " << totalLiquidVolume << " ml)\n";
        std::cout << "=================================\n";
    }
}
