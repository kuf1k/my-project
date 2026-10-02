#ifndef RECIPE_H
#define RECIPE_H
#include <vector>
#include <string>

namespace core {
    struct RecipeRequirement {
        std::string name;
        double requiredAmount;
        std::string unit; // "g" or "ml"
    };

    class Recipe {
        std::string title;
        size_t prepTimeMinutes;
        double calories = 0.0;
        std::string instructions;
        std::vector<RecipeRequirement> requirements;

    public:
        explicit Recipe(std::string title, size_t prepTimeMinutes = 0, double calories = 0.0,
                        std::string instructions = "");

        void addRequirement(const std::string &name, double amount, const std::string &unit);

        std::string getTitle() const;

        size_t getPrepTime() const;

        double getCalories() const;

        std::string getInstructions() const;

        const std::vector<RecipeRequirement> &getRequirements() const;
    };
}

#endif //RECIPE_H
