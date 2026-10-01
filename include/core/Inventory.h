#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include <memory>
#include "core/Ingredient.h"


namespace core {
    class Inventory {
        std::vector<std::unique_ptr<Ingredient> > items;

    public:
        Inventory() = default;

        ~Inventory() = default;

        void addIngredient(std::unique_ptr<Ingredient> ingredient);

        void printInventory(const std::string &currentDate) const;

        size_t getItemsCount() const;

        size_t removeExpired(const std::string &currentDate);

        void findByName(const std::string &query, const std::string &currentDate) const;

        void printExpiringSoon(const std::string currentDate) const;
    };
}

#endif //NVENTORY_H
