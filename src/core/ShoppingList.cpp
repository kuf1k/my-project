#include "core/ShoppingList.h"
#include <map>
#include <iostream>

namespace core {
    void ShoppingList::generateForRecipe(const Recipe &recipe, const Inventory &inventory) {
        std::vector<Recipe> singleRecipe = {recipe};
        generateForRecipes(singleRecipe, inventory);
    }

    void ShoppingList::generateForRecipes(const std::vector<Recipe> &recipes, const Inventory inventory) {
        clear();

        struct RequiredIitem {
            double amount = 0.0;
            std::string unit;
        };
        std::map<std::string, RequiredIitem> totalRequirements;
        for (const auto& recipe : recipes) {
            for (const auto& req : recipe.getRequirements()) {
                totalRequirements[req.name].amount += req.requiredAmount;
                totalRequirements[req.name].unit = req.unit;
            }
        }
        for (const auto& [name, req] : totalRequirements) {
            double available = inventory.getIngredientAmount(name);

            if (available < req.amount) {
                double shortage = req.amount - available;
                items.push_back({name, shortage, req.unit});
            }
        }
    }
    const std::vector<ShoppingListItem>& ShoppingList::getItems() const {
        return items;
    }

    void ShoppingList::printList() const {
        std::cout << "\n=== Shopping List ===\n";
        if (items.empty()) {
            std::cout << "Great news! You have all the ingredients in your inventory!\n";
        } else {
            for (const auto& item : items) {
                std::cout << "- " << item.name << ": need to buy "
                          << item.amountToBuy << " " << item.unit << "\n";
            }
        }
        std::cout << "===========================\n";
    }

    bool ShoppingList::isEmpty() const {
        return items.empty();
    }

    void ShoppingList::clear() {
        items.clear();
    }
}