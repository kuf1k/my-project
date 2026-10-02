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
        std::vector<RecipeRequirement> requirements;

    public:
        explicit Recipe(std::string title);

        void addRequirement(const std::string &name, double amount, const std::string &unit);

        std::string getTitle() const;

        const std::vector<RecipeRequirement> &getRequirements() const;
    };
}

#endif //RECIPE_H
