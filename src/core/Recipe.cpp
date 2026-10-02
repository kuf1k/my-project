#include "core/Recipe.h"
#include <utility>

namespace core {
    Recipe::Recipe(std::string title) : title(std::move(title)) {
    }

    void Recipe::addRequirement(const std::string &name, double amount, const std::string &unit) {
        requirements.push_back({name, amount, unit});
    }

    std::string Recipe::getTitle() const {
        return title;
    }

    const std::vector<core::RecipeRequirement> &core::Recipe::getRequirements() const {
        return requirements;
    }
}
