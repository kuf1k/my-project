#ifndef MEALPLANNER_H
#define MEALPLANNER_H

#include <vector>
#include <string>
#include "core/RecipeRepository.h"

namespace core {
    struct MealPlanEntry {
        std::string dayOfWeek;
        std::string time;
        std::string recipeTitle;
    };

    class MealPlanner {
        std::vector<MealPlanEntry> entries;

        static int getDayOrder(const std::string &day);

        void sortEntries();

    public:
        MealPlanner() = default;

        void addMeal(const std::string &dayOfWeek, const std::string &time, const std::string &recipeTitle);

        bool removeMealAt(size_t index);

        void clear();

        size_t getEntriesCount() const;

        const std::vector<MealPlanEntry> &getEntries() const;

        std::vector<Recipe> getPlannedRecipes(const RecipeRepository &repo) const;

        double getTotalCalories(const RecipeRepository &repo) const;
    };
}

#endif //MEALPLANNER_H
