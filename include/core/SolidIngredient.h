#ifndef SOLIDINGREDIENT_H
#define SOLIDINGREDIENT_H
#include "Ingredient.h"
#include <iostream>

namespace core {
    class SolidIngredient : public Ingredient {
    public:
        SolidIngredient(const std::string &name, double amount, const std::string &expirationDate);

        std::string getUnit() const override;

        void printInfo() const override;
    };
}

#endif //SOLIDINGREDIENT_H
