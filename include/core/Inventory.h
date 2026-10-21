#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include <memory>
#include "core/Ingredient.h"
#include "core/Recipe.h"


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

        void printStatistics() const;

        double getIngredientAmount(const std::string &name) const;

        bool consumeRecipe(const Recipe &recipe);

        void clear();

        const std::vector<std::unique_ptr<Ingredient> > &getItems() const;
    };
}

#endif //NVENTORY_H
