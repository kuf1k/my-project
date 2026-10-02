#include "core/Recipe.h"
#include <utility>

namespace core {
    Recipe::Recipe(std::string title, size_t prepTimeMinutes, double calories, std::string instructions)
        : title(std::move(title)),
          prepTimeMinutes(prepTimeMinutes),
          calories(calories),
          instructions(std::move(instructions)) {
    }

    void Recipe::addRequirement(const std::string &name, double amount, const std::string &unit) {
        requirements.push_back({name, amount, unit});
    }

    std::string Recipe::getTitle() const {
        return title;
    }

    size_t Recipe::getPrepTime() const {
        return prepTimeMinutes;
    }

    double Recipe::getCalories() const {
        return calories;
    }

    std::string Recipe::getInstructions() const {
        return instructions;
    }


    const std::vector<core::RecipeRequirement> &core::Recipe::getRequirements() const {
        return requirements;
    }
}
