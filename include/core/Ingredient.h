
#ifndef INGREDIENT_H
#define INGREDIENT_H
#include <string>

namespace core {
    class Ingredient {
    protected:
        std::string name;
        double amount = 0.0;
        std::string expirationDate;
    public:
        Ingredient(const std::string& name, double amount, const std::string& expirationDate);
        virtual ~Ingredient() = default;

        std::string getName() const;
        double getAmount() const;
        std::string getExpirationDate() const;

        void setAmount(double newAmount);
        virtual std::string getUnit() const = 0;
        virtual void printInfo() const = 0;

    };
}
#endif //INGREDIENT_H