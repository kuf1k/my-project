#include "core/MealPlanner.h"

namespace core {
    int MealPlanner::getDayOrder(const std::string &day) {
        if (day == "Monday") return 1;
        if (day == "Tuesday") return 2;
        if (day == "Wednesday") return 3;
        if (day == "Thursday") return 4;
        if (day == "Friday") return 5;
        if (day == "Saturday") return 6;
        if (day == "Sunday") return 7;
        return 8;
    }

    void MealPlanner::sortEntries() {
        std::sort(entries.begin(), entries.end(), [](const MealPlanEntry &a, const MealPlanEntry &b) {
            int dayA = getDayOrder(a.dayOfWeek);
            int dayB = getDayOrder(b.dayOfWeek);
            if (dayA != dayB) {
                return dayA < dayB;
            }
            return a.time < b.time;
        });
    }

    void MealPlanner::addMeal(const std::string &dayOfWeek, const std::string &time, const std::string &recipeTitle) {
        if (dayOfWeek.empty() || time.empty() || recipeTitle.empty()) {
            return;
        }
        entries.push_back({dayOfWeek, time, recipeTitle});
        sortEntries();
    }

    bool MealPlanner::removeMealAt(size_t index) {
        if (index >= entries.size()) {
            return false;
        }
        entries.erase(entries.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }

    void MealPlanner::clear() {
        entries.clear();
    }

    size_t MealPlanner::getEntriesCount() const {
        return entries.size();
    }

    const std::vector<MealPlanEntry> &MealPlanner::getEntries() const {
        return entries;
    }

    std::vector<Recipe> MealPlanner::getPlannedRecipes(const RecipeRepository &repo) const {
        std::vector<Recipe> planned;
        for (const auto &entry: entries) {
            for (const auto &recipe: repo.getRecipes()) {
                if (recipe.getTitle() == entry.recipeTitle) {
                    planned.push_back(recipe);
                    break;
                }
            }
        }
        return planned;
    }

    double MealPlanner::getTotalCalories(const RecipeRepository &repo) const {
        double total = 0.0;
        for (const auto &recipe: getPlannedRecipes(repo)) {
            total += recipe.getCalories();
        }
        return total;
    }
}
