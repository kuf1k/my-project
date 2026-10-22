#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H
#include "core/Inventory.h"
#include "core/RecipeRepository.h"

namespace core {
    class StorageManager {
    public:
        static bool saveInventory(const Inventory &inventory, const std::string &filename);

        static bool loadInventory(Inventory &inventory, const std::string &filename);


        static bool saveRecipes(const RecipeRepository &repo, const std::string &filename);

        static bool loadRecipes(RecipeRepository &repo, const std::string &filename);
    };
}

#endif //STORAGEMANAGER_H
