#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H
#include <string>
#include <vector>
#include "core/Recipe.h"
#include "core/Inventory.h"

namespace core {
    struct ShoppingListItem {
        std::string name;
        double amountToBuy = 0.0;
        std::string unit;
    };

    class ShoppingList {
        std::vector<ShoppingList> items;

    public:
        ShoppingList() = default;

        void genereateForRecipe(const Recipe &recipe, const Inventory &inventory);

        void genereateForRecipes(const std::vector<Recipe> &recipes, const Inventory inventory);

        const std::vector<ShoppingList> getItems() const;

        void printList() const;

        bool isEmpty() const;

        void clear();
    };
}
#endif //SHOPPINGLIST_H
