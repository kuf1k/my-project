#include "core/CountableIngredient.h"

namespace core {
    CountableIngredient::CountableIngredient(const std::string &name, double amount, const std::string &expirationDate)
        : Ingredient(name, amount, expirationDate) {
    }

    std::string CountableIngredient::getUnit() const {
        return "pcs";
    }

    void CountableIngredient::printInfo() const {
        std::cout << "[Countable] " << name << " : " << amount << getUnit()
                << " (expires : " << expirationDate << ")\n";
    }
}
