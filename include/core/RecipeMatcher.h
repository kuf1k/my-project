#ifndef RECIPEMATCHER_H
#define RECIPEMATCHER_H
#include "core/Recipe.h"
#include "core/Inventory.h"

namespace core {
    class RecipeMatcher {
    public:
        static bool canPrepare(const Recipe &recipe, const Inventory &inventory);

        static void checkRecipeAvailability(const Recipe &recipe, const Inventory &inventory);
    };
}
#endif //RECIPEMATCHER_H
