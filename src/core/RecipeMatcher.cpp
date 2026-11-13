#include "core/RecipeMatcher.h"
#include <iostream>

namespace core {
    bool RecipeMatcher::canPrepare(const Recipe &recipe, const Inventory &inventory,const std::string &currentDate) {
        for (const auto &req: recipe.getRequirements()) {
            double available = inventory.getIngredientAmount(req.name,req.unit, currentDate);
            if (available < req.requiredAmount) {
                return false;
            }
        }
        return true;
    }

    void RecipeMatcher::checkRecipeAvailability(const Recipe &recipe, const Inventory &inventory, const std::string &currentDate) {
        std::cout << "\n--- Recipe Check ---\n";
        std::cout << recipe.getTitle() << ": \n";
        bool fullyAvailable = true;

        for (const auto &req: recipe.getRequirements()) {
            double available = inventory.getIngredientAmount(req.name, req.unit);
            if (available >= req.requiredAmount) {
                std::cout << "  [OK] " << req.name << ": " << available << "/"
                        << req.requiredAmount << " " << req.unit << '\n';
            } else {
                fullyAvailable = false;
                double missing = req.requiredAmount - available;
                std::cout << "  [MISSING] " << req.name << ": need " << req.requiredAmount
                        << " " << req.unit << " (Available: " << available
                        << " " << req.unit << ", Missing: " << missing << " " << req.unit << ")\n";
            }
        }

        if (fullyAvailable) {
            std::cout << "Result: You can prepare " << recipe.getTitle() << "!\n";
        } else {
            std::cout << "Result: You do not have enough ingredients for " << recipe.getTitle() << "!\n";
        }
    }
}
