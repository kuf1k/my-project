#ifndef INGREDIENTVALIDATOR_H
#define INGREDIENTVALIDATOR_H

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>

namespace core {
    struct ValidationResult {
        bool isKnown = true;
        std::string expectedType;
        std::string expectedUnit;
        std::optional<std::string> typoSuggestion;
    };

    class IngredientValidator {
    public:
        IngredientValidator();

        ValidationResult validate(const std::string &name) const;

        std::vector<std::string> getKnownIngredientNames() const;

        static std::string getUnitForType(const std::string &type);

    private:
        struct ProductInfo {
            std::string canonicalName;
            std::string type;
        };

        std::unordered_map<std::string, ProductInfo> dictionary;

        static std::string toLower(const std::string &str);

        static int levenshteinDistance(const std::string &s1, const std::string &s2);
    };
};

#endif // INGREDIENTVALIDATOR_H
