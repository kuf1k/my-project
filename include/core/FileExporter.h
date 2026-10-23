#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H
#include "InventoryExporter.h"
#include "ShoppingList.h"

namespace core {
    class FileExporter : public InventoryExporter {
        std::string filePath;

    public:
        FileExporter(std::string title, std::string filePath);

        bool exportData(const Inventory &inventory, const std::string &currentDate) override;

        std::string getFilePath() const;

        static bool exportShoppingListToTxt(const ShoppingList &shoppingList, const std::string &filename);
    };
}

#endif //FILEEXPORTER_H
