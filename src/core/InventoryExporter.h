#ifndef INVENTORYEXPORTER_H
#define INVENTORYEXPORTER_H

#include <string>
#include "core/Inventory.h"

namespace core {
    class InventoryExporter {
    protected:
        std::string reportTitle;

    public:
        explicit InventoryExporter(std::string title);
        virtual ~InventoryExporter() = default;

        virtual bool exportData(const Inventory &inventory, const std::string &currentDate) = 0;

        std::string getReportTitle() const;
    };
}

#endif // INVENTORYEXPORTER_H