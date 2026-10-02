#include "core/RecipeRepository.h"
#include "core/RecipeMatcher.h"

namespace core {
    void RecipeRepository::addRecipe(const Recipe &recipe) {
        recipes.push_back(recipe);
    }

    void RecipeRepository::printAllRecipes() const {
        std::cout << "\n=== Recipe Book ===\n";
        if (recipes.empty()) {
            std::cout << "Your recipe book is empty!\n";
            return;
        }
        for (const auto &recipe: recipes) {
            std::cout << "- " << recipe.getTitle()
                    << " (" << recipe.getPrepTime() << " min, "
                    << recipe.getCalories() << " kcal)\n";
        }
    }

    std::vector<Recipe> RecipeRepository::getCookableRecipes(const Inventory &inventory) const {
        std::vector<Recipe> cookable;
        for (const auto &recipe: recipes) {
            if (RecipeMatcher::canPrepare(recipe, inventory)) {
                cookable.push_back(recipe);
            }
        }
        return cookable;
    }

    size_t RecipeRepository::getRecipesCount() const {
        return recipes.size();
    }
}