#include "core/Inventory.h"
#include "core/DateUtils.h"
#include <iostream>

namespace core {
    void Inventory::addIngredient(std::unique_ptr<Ingredient> ingredient) {
        if (ingredient) {
            items.push_back(std::move(ingredient));
        }
    }

    void Inventory::printInventory(const std::string& currentDate) const {
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
}
