#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H
#include "core/Inventory.h"

namespace core {
    class StorageManager {
    public:
        static bool saveInvertory(const Inventory &inventory, const std::string &filename);

        static bool loadInventory(Inventory &inventory, const std::string &filename);
    };
}

#endif //STORAGEMANAGER_H
