#include "core/IngredientValidator.h"
#include <algorithm>
#include <cctype>

namespace core {

IngredientValidator::IngredientValidator() {
    auto addEntry = [this](const std::string &canonical, const std::string &type) {
        dictionary[toLower(canonical)] = {canonical, type};
    };

    addEntry("Eggs", "Countable");
    addEntry("Egg", "Countable");
    addEntry("Apples", "Countable");
    addEntry("Apple", "Countable");
    addEntry("Bananas", "Countable");
    addEntry("Banana", "Countable");
    addEntry("Tomatoes", "Countable");
    addEntry("Tomato", "Countable");
    addEntry("Potatoes", "Countable");
    addEntry("Potato", "Countable");
    addEntry("Onions", "Countable");
    addEntry("Onion", "Countable");
    addEntry("Carrots", "Countable");
    addEntry("Carrot", "Countable");
    addEntry("Lemons", "Countable");
    addEntry("Lemon", "Countable");
    addEntry("Oranges", "Countable");
    addEntry("Orange", "Countable");
    addEntry("Cucumbers", "Countable");
    addEntry("Cucumber", "Countable");
    addEntry("Garlic", "Countable");
    addEntry("Bread", "Countable");

    addEntry("Milk", "Liquid");
    addEntry("Water", "Liquid");
    addEntry("Olive Oil", "Liquid");
    addEntry("Sunflower Oil", "Liquid");
    addEntry("Oil", "Liquid");
    addEntry("Cream", "Liquid");
    addEntry("Soy Sauce", "Liquid");
    addEntry("Vinegar", "Liquid");
    addEntry("Juice", "Liquid");
    addEntry("Orange Juice", "Liquid");
    addEntry("Honey", "Liquid");
    addEntry("Kefir", "Liquid");
    addEntry("Yogurt", "Liquid");

    addEntry("Flour", "Solid");
    addEntry("Sugar", "Solid");
    addEntry("Salt", "Solid");
    addEntry("Butter", "Solid");
    addEntry("Cheese", "Solid");
    addEntry("Rice", "Solid");
    addEntry("Pasta", "Solid");
    addEntry("Buckwheat", "Solid");
    addEntry("Oatmeal", "Solid");
    addEntry("Chicken Breast", "Solid");
    addEntry("Chicken", "Solid");
    addEntry("Beef", "Solid");
    addEntry("Pork", "Solid");
    addEntry("Fish", "Solid");
    addEntry("Salmon", "Solid");
    addEntry("Cottage Cheese", "Solid");
    addEntry("Sour Cream", "Solid");
    addEntry("Chocolate", "Solid");
    addEntry("Cocoa", "Solid");
    addEntry("Yeast", "Solid");
    addEntry("Pepper", "Solid");
}

ValidationResult IngredientValidator::validate(const std::string &name) const {
    ValidationResult result;
    std::string lowerName = toLower(name);

    if (lowerName.empty()) {
        return result;
    }

    auto it = dictionary.find(lowerName);
    if (it != dictionary.end()) {
        result.isKnown = true;
        result.expectedType = it->second.type;
        result.expectedUnit = getUnitForType(it->second.type);
        return result;
    }

    int bestDistance = 3;
    std::string bestMatch;

    for (const auto &[key, info] : dictionary) {
        if (lowerName.size() < 3 || key.size() < 3) {
            continue;
        }
        int dist = levenshteinDistance(lowerName, key);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestMatch = info.canonicalName;
        }
    }

    if (!bestMatch.empty()) {
        result.typoSuggestion = bestMatch;
    }

    return result;
}

std::vector<std::string> IngredientValidator::getKnownIngredientNames() const {
    std::vector<std::string> names;
    names.reserve(dictionary.size());
    for (const auto &[key, info] : dictionary) {
        names.push_back(info.canonicalName);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::string IngredientValidator::getUnitForType(const std::string &type) {
    if (type == "Solid") return "g";
    if (type == "Liquid") return "ml";
    if (type == "Countable") return "pcs";
    return "";
}

std::string IngredientValidator::toLower(const std::string &str) {
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return res;
}

int IngredientValidator::levenshteinDistance(const std::string &s1, const std::string &s2) {
    const size_t m = s1.size();
    const size_t n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (size_t i = 0; i <= m; ++i) dp[i][0] = static_cast<int>(i);
    for (size_t j = 0; j <= n; ++j) dp[0][j] = static_cast<int>(j);

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + cost
            });
        }
    }
    return dp[m][n];
}

}
