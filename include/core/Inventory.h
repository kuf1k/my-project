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

        void printInventory() const;

        size_t getItemsCount() const;
    };
}

#endif //NVENTORY_H
