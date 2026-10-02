#include "core/RecipeMatcher.h"
#include <iostream>
namespace core {
    bool RecipeMatcher::canPrepare(const Recipe &recipe, const Inventory &inventory) {
        for (const auto &req : recipe.getRequirements()) {
            double available = inventory.getIngredientAmount(req.name);
            if (available < req.requiredAmount) {
                return false;
            }
        }
        return true;
    }
    void RecipeMatcher::checkRecipeAvailability(const Recipe &recipe, const Inventory &inventory) {
        std::cout << "\n--- Recipe Check: " << recipe.getTitle() << " ---\n";
        bool fullyAvailable = true;

        for (const auto &req : recipe.getRequirements()) {
            double available = inventory.getIngredientAmount(req.name);
            if (available >= req.requiredAmount) {
                std::cout << "  [OK] " << req.name << ": " << available << "/"
                          << req.requiredAmount << " " << req.unit << '\n';
            } else {
                fullyAvailable = false;
                double missing = req.requiredAmount - available;
                std::cout << "  [MISSING] " << req.name << ": need " << req.requiredAmount
                          << " " << req.unit << " (Available: " << available
                          << " " << req.unit << ", Short: " << missing << " " << req.unit << ")\n";
            }
        }

        if (fullyAvailable) {
            std::cout << "Result: You can prepare " << recipe.getTitle() << " right now!\n";
        } else {
            std::cout << "Result: You are missing ingredients for " << recipe.getTitle() << ".\n";
        }
    }
}