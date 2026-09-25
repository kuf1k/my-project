#include "core/LiquidIngredient.h"
#include <iostream>

namespace core {
    LiquidIngredient::LiquidIngredient(const std::string &name, double amount, const std::string &expirationDate)
        : Ingredient(name, amount, expirationDate) {
    }

    std::string LiquidIngredient::getUnit() const {
        return "ml";
    }

    void LiquidIngredient::printInfo() const {
        std::cout << "[Liquid] " << name << " : " << amount << getUnit()
                << " (expires : " << expirationDate << ")\n";
    }
}
