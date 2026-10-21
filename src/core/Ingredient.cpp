#include "core/Ingredient.h"

namespace core {
    Ingredient::Ingredient(const std::string &name, double amount, const std::string &expirationDate)
        : name(name), amount(amount), expirationDate(expirationDate) {
    }

    std::string Ingredient::getName() const {
        return name;
    }

    double Ingredient::getAmount() const {
        return amount;
    }

    std::string Ingredient::getExpirationDate() const {
        return expirationDate;
    }

    void Ingredient::setAmount(double newAmount) {
        if (newAmount >= 0.0) {
            amount = newAmount;
        }
    }

    void Ingredient::reduceAmount(double value) {
        if (value >= amount) {
            amount = 0;
        } else {
            amount -= value;
        }
    }


    bool Ingredient::isExpired(const std::string &currentDate) const {
        if (DateUtils::calculateStatus(expirationDate, currentDate) == FreshnessStatus::Expired) {
            return true;
        }
        return false;
    }


    FreshnessStatus Ingredient::getFreshnessStatus(const std::string &currentDate) const {
        return DateUtils::calculateStatus(expirationDate, currentDate);
    }
}
