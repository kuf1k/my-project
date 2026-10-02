#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H
#include "InventoryExporter.h"

namespace core {
    class FileExporter : public InventoryExporter {
        std::string filePath;

    public:
        FileExporter(std::string title, std::string filePath);

        bool exportData(const Inventory &inventory, const std::string &currentDate) override;

        std::string getFilePath() const;
    };
}

#endif //FILEEXPORTER_H
