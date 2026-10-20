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
        std::vector<ShoppingListItem> items;

    public:
        ShoppingList() = default;

        void generateForRecipe(const Recipe &recipe, const Inventory &inventory);

        void generateForRecipes(const std::vector<Recipe> &recipes, const Inventory inventory);

        const std::vector<ShoppingListItem>& getItems() const;

        void printList() const;

        bool isEmpty() const;

        void clear();
    };
}
#endif //SHOPPINGLIST_H
