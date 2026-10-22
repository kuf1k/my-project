#ifndef COUNTABLEINGREDIENT_H
#define COUNTABLEINGREDIENT_H
#include "core/Ingredient.h"

namespace core {
    class CountableIngredient : public Ingredient {
    public:
        CountableIngredient(const std::string &name, double amount, const std::string &expirationDate);

        std::string getUnit() const override;

        void printInfo() const override;
    };
}

#endif //COUNTABLEINGREDIENT_H
