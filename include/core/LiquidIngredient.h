#ifndef LIQUID_INGREDIENT_H
#define LIQUID_INGREDIENT_H
#include "Ingredient.h"

namespace core {
    class LiquidIngredient : public Ingredient {
    public:
        LiquidIngredient(const std::string &name, double amount, const std::string &expirationDate);

        std::string getUnit() const override;

        void printInfo() const override;
    };
}

#endif //LIQUID_INGREDIENT_H
