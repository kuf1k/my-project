#include "core/Inventory.h"
#include "core/DateUtils.h"
#include "core/RecipeMatcher.h"
#include <iostream>
#include <vector>


namespace core {
    void Inventory::addIngredient(std::unique_ptr<Ingredient> ingredient) {
        if (!ingredient) return;

        auto toLower = [](const std::string &str) {
            std::string res = str;
            std::transform(res.begin(), res.end(), res.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return res;
        };

        std::string newNameLower = toLower(ingredient->getName());

        for (auto &item: items) {
            if (toLower(item->getName()) == newNameLower &&
                item->getExpirationDate() == ingredient->getExpirationDate() &&
                item->getUnit() == ingredient->getUnit()) {
                item->setAmount(item->getAmount() + ingredient->getAmount());
                return;
            }
        }
        items.push_back(std::move(ingredient));
    }

    void Inventory::printInventory(const std::string &currentDate) const {
        std::cout << "\n=== Your Inventory ===\n";
        if (items.empty()) {
            std::cout << " Your Inventory is empty!\n";
            return;
        }
        for (const auto &item: items) {
            item->printInfo();

            FreshnessStatus status = item->getFreshnessStatus(currentDate);
            std::cout << "Status:" << DateUtils::getStatusLabel(status) << '\n';
        }
    }

    size_t Inventory::getItemsCount() const {
        return items.size();
    }

    size_t Inventory::removeExpired(const std::string &currentDate) {
        size_t removedCount = (std::erase_if(items, [&](const std::unique_ptr<Ingredient> &item) {
            return item->getFreshnessStatus(currentDate) == FreshnessStatus::Expired;
        }));
        return removedCount;
    }

    bool Inventory::removeIngredientAt(size_t index) {
        if (index >= items.size()) {
            return false;
        }
        items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }

    void Inventory::findByName(const std::string &query, const std::string &currentDate) const {
        std::cout << "\n=== Search Results for '" << query << "' ===\n";
        bool found = false;
        for (const auto &item: items) {
            if (item->getName().find(query) != std::string::npos) {
                item->printInfo();
                FreshnessStatus status = item->getFreshnessStatus(currentDate);
                std::cout << "Status: " << DateUtils::getStatusLabel(status);
                found = true;
            }
        }
        if (!found) {
            std::cout << "No ingredients found matching " << query << "\n";
        }
    }

    void Inventory::printExpiringSoon(const std::string currentDate) const {
        std::cout << "\n=== Ingredients Expiring Soon ===\n";
        bool found = false;

        for (const auto &item: items) {
            if (item->getFreshnessStatus(currentDate) == FreshnessStatus::ExpiringSoon) {
                item->printInfo();
                FreshnessStatus status = item->getFreshnessStatus(currentDate);
                std::cout << "Status: " << DateUtils::getStatusLabel(status);
                found = true;
            }
        }
        if (!found) {
            std::cout << "No ingredients expiring soon!\n";
        }
    }

    void Inventory::printStatistics() const {
        double totalSolidWeight = 0.0;
        double totalLiquidVolume = 0.0;
        double totalCountableQuantity = 0.0;
        size_t solidCount = 0;
        size_t liquidCount = 0;
        size_t countableCount = 0;

        for (const auto &item: items) {
            if (item->getUnit() == "g") {
                totalSolidWeight += item->getAmount();
                solidCount++;
            } else if (item->getUnit() == "ml") {
                totalLiquidVolume += item->getAmount();
                liquidCount++;
            } else if (item->getUnit() == "pcs") {
                totalCountableQuantity += item->getAmount();
                countableCount++;
            }
        }

        std::cout << "\n=================================\n";
        std::cout << "      Inventory Statistics       \n";
        std::cout << "=================================\n";
        std::cout << "Total Ingredients: " << items.size() << '\n';
        std::cout << "Solid Ingredients: " << solidCount << " (Total Weight: " << totalSolidWeight << " g)\n";
        std::cout << "Liquid Ingredients: " << liquidCount << " (Total Volume: " << totalLiquidVolume << " ml)\n";
        std::cout << "Countable Ingredients: " << countableCount << " (Total Quantity: " << totalCountableQuantity <<
                " pcs)\n";
        std::cout << "=================================\n";
    }

    double Inventory::getIngredientAmount(const std::string &name) const {
        double total = 0.0;
        for (const auto &item: items) {
            if (item->getName() == name) {
                total += item->getAmount();
            }
        }
        return total;
    }

    double Inventory::getIngredientAmount(const std::string &name, const std::string &unit, const std::string &currentDate) const {
        auto toLower = [](const std::string &str) {
            std::string res = str;
            std::transform(res.begin(), res.end(), res.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return res;
        };

        std::string targetName = toLower(name);
        double total = 0.0;
        for (const auto &item: items) {
            if (toLower(item->getName()) == targetName && item->getUnit() == unit) {
                if (!currentDate.empty() && item->getFreshnessStatus(currentDate) == FreshnessStatus::Expired) {
                    continue;
                }
                total += item->getAmount();
            }
        }
        return total;
    }

    bool Inventory::consumeRecipe(const Recipe &recipe, const std::string &currentDate) {
        if (!RecipeMatcher::canPrepare(recipe, *this, currentDate)) {
            return false;
        }

        std::sort(items.begin(), items.end(),
                  [](const std::unique_ptr<Ingredient> &a, const std::unique_ptr<Ingredient> &b) {
                      return a->getExpirationDate() < b->getExpirationDate();
                  });

        auto toLower = [](const std::string &str) {
            std::string res = str;
            std::transform(res.begin(), res.end(), res.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return res;
        };

        for (const auto &req: recipe.getRequirements()) {
            double amountNeeded = req.requiredAmount;
            std::string reqNameLower = toLower(req.name);

            for (auto it = items.begin(); it != items.end() && amountNeeded > 0;) {
                bool isExpired = !currentDate.empty() &&
                                 (*it)->getFreshnessStatus(currentDate) == FreshnessStatus::Expired;

                if (!isExpired && toLower((*it)->getName()) == reqNameLower && (*it)->getUnit() == req.unit) {
                    double available = (*it)->getAmount();

                    if (available <= amountNeeded) {
                        amountNeeded -= available;
                        it = items.erase(it);
                    } else {
                        (*it)->reduceAmount(amountNeeded);
                        amountNeeded = 0;
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }
        return true;
    }

    void Inventory::clear() {
        items.clear();
    }

    const std::vector<std::unique_ptr<Ingredient> > &Inventory::getItems() const {
        return items;
    }
}
