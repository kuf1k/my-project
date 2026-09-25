#include "core/SolidIngredient.h"
#include <iostream>
namespace core {
    SolidIngredient::SolidIngredient(const std::string &name, double amount, const std::string &expirationDate)
        : Ingredient(name,amount,expirationDate) {}

    std::string SolidIngredient::getUnit() const {
        return "g"; // Grams
    }

    void SolidIngredient::printInfo() const {
        std::cout << "[Solid] " << name << " : " << amount << getUnit()
        << " (expires : " << expirationDate << ")\n";
    }
}