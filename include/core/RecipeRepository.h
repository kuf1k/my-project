#ifndef RECIPEREPOSITORY_H
#define RECIPEREPOSITORY_H
#include <vector>
#include "Recipe.h"
#include "core/Inventory.h"

namespace core {
    enum class RecipeSortOption {
        ByNameAsc,
        ByPrepTimeAsc,
        ByCaloriesAsc,
        ByCaloriesDesc
    };

    class RecipeRepository {
        std::vector<Recipe> recipes;

    public:
        RecipeRepository() = default;

        void addRecipe(const Recipe &recipe);

        void printAllRecipes() const;

        std::vector<Recipe> getCookableRecipes(const Inventory &inventory) const;

        size_t getRecipesCount() const;

        bool hasRecipe(const std::string &title) const;

        const std::vector<Recipe> &getRecipes() const;

        std::vector<Recipe> getSortedRecipes(RecipeSortOption option) const;
    };
}


#endif //RECIPEREPOSITORY_H
