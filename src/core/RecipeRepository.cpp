#include "core/RecipeRepository.h"
#include "core/RecipeMatcher.h"

namespace core {
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

    bool RecipeRepository::hasRecipe(const std::string &title) const {
        return std::find_if(recipes.begin(), recipes.end(),
                            [&title](const Recipe &recipe) {
                                return recipe.getTitle() == title;
                            }) != recipes.end();
    }

    const std::vector<Recipe> &RecipeRepository::getRecipes() const {
        return recipes;
    }
    void RecipeRepository::addRecipe(const Recipe &recipe) {
        if (hasRecipe(recipe.getTitle())) {
            std::cout << "Recipe with title '" << recipe.getTitle() << "' already exists. Skipping.\n";
            return;
        }
        recipes.push_back(recipe);
    }
}
