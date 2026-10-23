#include "core/RecipeRepository.h"
#include "core/RecipeMatcher.h"

namespace core {
    void RecipeRepository::printAllRecipes() const {
        std::cout << "\n========================================\n";
        std::cout << "             Recipe Book                \n";
        std::cout << "========================================\n";
        if (recipes.empty()) {
            std::cout << "Your recipe book is empty!\n";
            return;
        }
        for (const auto &recipe: recipes) {
            std::cout << "- " << recipe.getTitle()
                    << " (" << recipe.getPrepTime() << " min, "
                    << recipe.getCalories() << " kcal)\n";
            if (!recipe.getInstructions().empty()) {
                std::cout << "  Instructions: " << recipe.getInstructions() << "\n";
            }

            const auto &requirements = recipe.getRequirements();
            if (!requirements.empty()) {
                std::cout << "  Ingredients required:\n";
                for (const auto &req: requirements) {
                    std::cout << "    - " << req.name << ": "
                            << req.requiredAmount << " " << req.unit << "\n";
                }
            }
            std::cout << "========================================\n";
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

    std::vector<Recipe>
    RecipeRepository::sortRecipes(const std::vector<Recipe> &recipesToSort, RecipeSortOption option) {
        std::vector<Recipe> sorted = recipesToSort;
        switch (option) {
            case RecipeSortOption::ByNameAsc:
                std::sort(sorted.begin(), sorted.end(), [](const Recipe &a, const Recipe &b) {
                    return a.getTitle() < b.getTitle();
                });
                break;
            case RecipeSortOption::ByPrepTimeAsc:
                std::sort(sorted.begin(), sorted.end(), [](const Recipe &a, const Recipe &b) {
                    return a.getPrepTime() < b.getPrepTime();
                });
                break;
            case RecipeSortOption::ByCaloriesAsc:
                std::sort(sorted.begin(), sorted.end(), [](const Recipe &a, const Recipe &b) {
                    return a.getCalories() < b.getCalories();
                });
                break;
            case RecipeSortOption::ByCaloriesDesc:
                std::sort(sorted.begin(), sorted.end(), [](const Recipe &a, const Recipe &b) {
                    return a.getCalories() > b.getCalories();
                });
                break;
        }
        return sorted;
    }

    std::vector<Recipe> RecipeRepository::getSortedRecipes(RecipeSortOption option) const {
        return sortRecipes(recipes, option);
    }
}
