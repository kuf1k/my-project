#include "core/Inventory.h"
#include <iostream>

namespace core {
    void Inventory::addIngredient(std::unique_ptr<Ingredient> ingredient) {
        if (ingredient) {
            items.push_back(std::move(ingredient));
        }
    }

    void Inventory::printInventory() const {
        std::cout << "\n=== Your Inventory ===\n";
        if (items.empty()) {
            std::cout << " Your Inventory is empty!\n";
            return;
        }
        for (const auto &item: items) {
            item->printInfo();
        }
    }

    size_t Inventory::getItemsCount() const {
        return items.size();
    }
}
