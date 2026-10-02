#include "core/InventoryExporter.h"

namespace core {
    InventoryExporter::InventoryExporter(std::string title)
        : reportTitle(std::move(title)) {}

    std::string InventoryExporter::getReportTitle() const {
        return reportTitle;
    }
}