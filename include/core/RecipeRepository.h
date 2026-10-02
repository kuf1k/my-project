#ifndef RECIPEREPOSITORY_H
#define RECIPEREPOSITORY_H
#include <vector>
#include "Recipe.h"
#include "core/Inventory.h"

namespace core {
    class RecipeRepository {
        std::vector<Recipe> recipes;

    public:
        RecipeRepository() = default;

        void addRecipe(const Recipe &recipe);

        void printAllRecipes() const;

        std::vector<Recipe> getCookableRecipes(const Inventory &inventory) const;

        size_t getRecipesCount() const;
    };
}


#endif //RECIPEREPOSITORY_H
