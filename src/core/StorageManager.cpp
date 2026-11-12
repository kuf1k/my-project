#include "core/StorageManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

#include "core/CountableIngredient.h"

using json = nlohmann::json;

namespace core {
    bool StorageManager::saveInventory(const Inventory &inventory, const std::string &filename) {
        json jArray = json::array();

        for (const auto &item: inventory.getItems()) {
            json itemObj;
            if (item->getUnit() == "g") {
                itemObj["type"] = "Solid";
            } else if (item->getUnit() == "ml") {
                itemObj["type"] = "Liquid";
            } else if (item->getUnit() == "pcs") {
                itemObj["type"] = "Countable";
            }
            itemObj["name"] = item->getName();
            itemObj["amount"] = item->getAmount();
            itemObj["expirationDate"] = item->getExpirationDate();

            jArray.push_back(itemObj);
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            spdlog::error("Failed to open file for saving inventory: {}", filename);
            return false;
        }

        file << jArray.dump(4);
        file.close();

        spdlog::info("Successfully saved {} inventory items to {}", inventory.getItemsCount(), filename);
        return true;
    }

    bool StorageManager::loadInventory(Inventory &inventory, const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            spdlog::warn("Inventory save file not found or failed to open: {}", filename);
            return false;
        }


        json jArray;
        try {
            file >> jArray;
        } catch (const std::exception &e) {
            file.close();
            spdlog::error("JSON parse error while loading inventory from {}: {}", filename, e.what());
            return false;
        }

        file.close();
        inventory.clear();

        for (const auto &itemObj: jArray) {
            std::string type = itemObj.value("type", "Countable");
            std::string name = itemObj.value("name", "");
            double amount = itemObj.value("amount", 0.0);
            std::string expirationDate = itemObj.value("expirationDate", "");

            if (type == "Solid") {
                inventory.addIngredient(std::make_unique<SolidIngredient>(name, amount, expirationDate));
            } else if (type == "Liquid") {
                inventory.addIngredient(std::make_unique<LiquidIngredient>(name, amount, expirationDate));
            } else if (type == "Countable") {
                inventory.addIngredient(std::make_unique<CountableIngredient>(name, amount, expirationDate));
            }
        }
        spdlog::info("Successfully loaded {} inventory items from {}", inventory.getItemsCount(), filename);
        return true;
    }

    bool StorageManager::saveRecipes(const RecipeRepository &repo, const std::string &filename) {
        json jArray = json::array();

        for (const auto &recipe: repo.getRecipes()) {
            json recipeJson;
            recipeJson["title"] = recipe.getTitle();
            recipeJson["prepTimeMinutes"] = recipe.getPrepTime();
            recipeJson["calories"] = recipe.getCalories();
            recipeJson["instructions"] = recipe.getInstructions();

            json reqArray = json::array();
            for (const auto &req: recipe.getRequirements()) {
                json reqJson;
                reqJson["name"] = req.name;
                reqJson["requiredAmount"] = req.requiredAmount;
                reqJson["unit"] = req.unit;
                reqArray.push_back(reqJson);
            }
            recipeJson["requirements"] = reqArray;

            jArray.push_back(recipeJson);
        }

        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            spdlog::error("Failed to open file for saving recipes: {}", filename);
            return false;
        }

        outFile << jArray.dump(4);
        outFile.close();

        spdlog::info("Successfully saved {} recipes to {}", repo.getRecipesCount(), filename);
        return true;
    }

    bool StorageManager::loadRecipes(RecipeRepository &repo, const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            spdlog::warn("Recipes save file not found or failed to open: {}", filename);
            return false;
        }

        json jArray;
        try {
            inFile >> jArray;
        } catch (const json::parse_error &e) {
            inFile.close();
            spdlog::error("JSON parse error while loading recipes from {}: {}", filename, e.what());
            return false;
        }

        for (const auto &recipeJson: jArray) {
            std::string title = recipeJson.value("title", "");
            size_t prepTime = recipeJson.value("prepTimeMinutes", 0);
            double calories = recipeJson.value("calories", 0.0);
            std::string instructions = recipeJson.value("instructions", "");

            if (repo.hasRecipe(title)) {
                continue;
            }

            Recipe recipe(title, prepTime, calories, instructions);

            if (recipeJson.contains("requirements") && recipeJson["requirements"].is_array()) {
                for (const auto &reqJson: recipeJson["requirements"]) {
                    std::string reqName = reqJson.value("name", "");
                    double reqAmount = reqJson.value("requiredAmount", 0.0);
                    std::string reqUnit = reqJson.value("unit", "");

                    recipe.addRequirement(reqName, reqAmount, reqUnit);
                }
            }

            repo.addRecipe(recipe);
        }

        inFile.close();

        spdlog::info("Successfully loaded recipes from {}", filename);
        return true;
    }

    bool StorageManager::saveMealPlan(const MealPlanner &planner, const std::string &filename) {
        json jArray = json::array();

        for (const auto &entry: planner.getEntries()) {
            json entryJson;
            entryJson["dayOfWeek"] = entry.dayOfWeek;
            entryJson["time"] = entry.time;
            entryJson["recipeTitle"] = entry.recipeTitle;
            jArray.push_back(entryJson);
        }

        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            spdlog::error("Failed to open file for saving meal plan: {}", filename);
            return false;
        }

        outFile << jArray.dump(4);
        outFile.close();

        spdlog::info("Successfully saved {} meal plan entries to {}", planner.getEntriesCount(), filename);
        return true;
    }

    bool StorageManager::loadMealPlan(MealPlanner &planner, const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            spdlog::warn("Meal plan save file not found or failed to open: {}", filename);
            return false;
        }

        json jArray;
        try {
            inFile >> jArray;
        } catch (const std::exception &e) {
            inFile.close();
            spdlog::error("JSON parse error while loading meal plan from {}: {}", filename, e.what());
            return false;
        }

        inFile.close();
        planner.clear();

        for (const auto &entryJson: jArray) {
            std::string dayOfWeek = entryJson.value("dayOfWeek", "");
            std::string time = entryJson.value("time", "");
            std::string recipeTitle = entryJson.value("recipeTitle", "");

            planner.addMeal(dayOfWeek, time, recipeTitle);
        }

        spdlog::info("Successfully loaded {} meal plan entries from {}", planner.getEntriesCount(), filename);
        return true;
    }
}
