#include <iostream>
#include <memory>
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include "core/Inventory.h"
#include "core/Recipe.h"
#include "core/RecipeMatcher.h"
#include "core/DataFilter.h"
#include "core/FileExporter.h"
#include "core/RecipeRepository.h"
#include "core/ShoppingList.h"
#include "core/StorageManager.h"

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "     NutriMesh - Kitchen Assistant \n";
    std::cout << "========================================\n";
    std::cout << "1. Add Solid Ingredient (g)\n";
    std::cout << "2. Add Liquid Ingredient (ml)\n";
    std::cout << "3. View Inventory\n";
    std::cout << "4. Search Ingredient by Name\n";
    std::cout << "5. View Expiring Soon Ingredients\n";
    std::cout << "6. Remove Expired Ingredients\n";
    std::cout << "7. View Inventory Statistics\n";
    std::cout << "8. Check Recipe Availability\n";
    std::cout << "9. Export Inventory Report to File\n";
    std::cout << "10. Filter Recipes by Max Calories \n";
    std::cout << "11. Generate Shopping List for Recipe\n";
    std::cout << "12. Cook Recipe (Consume Ingredients)\n";
    std::cout << "13. Save Data to Files (Inventory & Recipes)\n";
    std::cout << "14. Load Data from Files (Inventory & Recipes)\n";
    std::cout << "15. Add Custom Recipe (Recipe Builder)\n";
    std::cout << "16. View All Recipes (Recipe Book)\n";
    std::cout << "17. View Cookable Recipes (What can I cook now?)\n";
    std::cout << "18. Exit\n";
    std::cout << "Choose an option (1-18): ";
}

double readAmount(const std::string &inputText) {
    double value;
    while (true) {
        std::cout << inputText;
        std::cin >> value;

        if (!std::cin.fail() && value > 0) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid non-negative number.\n";
    }
}

std::string readDate(const std::string &inputText) {
    std::string date;
    while (true) {
        std::cout << inputText;
        std::cin >> date;
        if (core::DateUtils::isValidDate(date)) {
            return date;
        }
        std::cout << "Invalid date format! Please enter YYYY-MM-DD (e.g. 2026-09-30):\n";
    }
}

void enterInfo(std::string &name, double &amount, std::string &expirationDate) {
    std::cout << "Enter name: ";
    std::cin >> name;
    amount = readAmount("Enter amount: ");
    expirationDate = readDate("Enter expiration date (YYYY-MM-DD): ");
}

void runExport(core::InventoryExporter &exporter, const core::Inventory &inventory, const std::string &date) {
    if (exporter.exportData(inventory, date)) {
        std::cout << "Successfully exported report: " << exporter.getReportTitle() << "!\n";
    } else {
        std::cout << "Failed to export report.\n";
    }
}

const core::Recipe *selectRecipe(const core::RecipeRepository &recipeRepo) {
    if (recipeRepo.getRecipesCount() == 0) {
        std::cout << "Recipe book is empty! Please add a recipe first.\n";
        return nullptr;
    }

    std::string title;
    std::cout << "Enter recipe title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);

    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            return &recipe;
        }
    }

    std::cout << "Recipe '" << title << "' not found in recipe book.\n";
    return nullptr;
}


int main() {
    core::Inventory inventory;
    core::RecipeRepository recipeRepo;
    bool running = true;

    core::StorageManager::loadInventory(inventory, "inventory_save.json");
    core::StorageManager::loadRecipes(recipeRepo, "recipes_save.json");


    // Test recipe
    core::Recipe pancakes("Pancakes", 15, 350.0, "Mix flour, milk and fry on medium heat.");
    pancakes.addRequirement("Flour", 200.0, "g");
    pancakes.addRequirement("Milk", 500.0, "ml");
    recipeRepo.addRecipe(pancakes);

    while (running) {
        displayMenu();
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            std::string name, expirationDate;
            double amount = 0;

            enterInfo(name, amount, expirationDate);

            auto item = std::make_unique<core::SolidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Solid ingredient has been successfully added!\n";
        } else if (choice == 2) {
            std::string name, expirationDate;
            double amount = 0;

            enterInfo(name, amount, expirationDate);

            auto item = std::make_unique<core::LiquidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Liquid ingredient has been successfully added!\n";
        } else if (choice == 3) {
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");
            inventory.printInventory(currentDate);
        } else if (choice == 4) {
            std::string query;
            std::cout << "Enter ingredient name (or part of it): ";
            std::cin >> query;
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");
            inventory.findByName(query, currentDate);
        } else if (choice == 5) {
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");
            inventory.printExpiringSoon(currentDate);
        } else if (choice == 6) {
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");
            size_t removed = inventory.removeExpired(currentDate);
            if (removed > 0) {
                std::cout << "Succesfully removed " << removed << " expired ingredient(s)!\n";
            } else {
                std::cout << "No expired ingredients\n";
            }
        } else if (choice == 7) {
            inventory.printStatistics();
        } else if (choice == 8) {
            const auto *recipe = selectRecipe(recipeRepo);
            if (recipe) {
                core::RecipeMatcher::checkRecipeAvailability(*recipe, inventory);
            }
        } else if (choice == 9) {
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");

            core::FileExporter fileExporter("NutriMesh Inventory Report", "inventory_report.txt");

            runExport(fileExporter, inventory, currentDate);
        } else if (choice == 10) {
            double maxCalories = readAmount("Enter maximum calorie limit: ");

            auto filteredRecipes = core::DataFilter<core::Recipe>::filterBy(
                recipeRepo.getRecipes(), [maxCalories](const core::Recipe &recipe) {
                    return recipe.getCalories() <= maxCalories;
                });

            if (filteredRecipes.empty()) {
                std::cout << "No recipes found under " << maxCalories << " kcal.\n";
            } else {
                std::cout << "\n=== Recipes under " << maxCalories << " kcal ===\n";
                for (const auto &recipe: filteredRecipes) {
                    std::cout << "- " << recipe.getTitle() << " (" << recipe.getCalories() << " kcal)\n";
                }
            }
        } else if (choice == 11) {
            const auto *recipe = selectRecipe(recipeRepo);
            if (recipe) {
                core::ShoppingList shopList;
                shopList.generateForRecipe(*recipe, inventory);
                shopList.printList();
            }
        } else if (choice == 12) {
            const auto *recipe = selectRecipe(recipeRepo);
            if (recipe) {
                if (inventory.consumeRecipe(*recipe)) {
                    std::cout << "Successfully cooked " << recipe->getTitle() <<
                            "! Ingredients were deducted from inventory.\n";
                } else {
                    std::cout << "Failed to cook " << recipe->getTitle() << ". Not enough ingredients in inventory!\n";
                }
            }
        } else if (choice == 13) {
            bool invSaved = core::StorageManager::saveInventory(inventory, "inventory_save.json");
            bool recSaved = core::StorageManager::saveRecipes(recipeRepo, "recipes_save.json");
            if (invSaved && recSaved) {
                std::cout << "Inventory and recipes successfully saved to JSON files!\n";
            } else if (invSaved) {
                std::cout << "Inventory saved, but failed to save recipes.\n";
            } else {
                std::cout << "Failed to save data.\n";
            }
        } else if (choice == 14) {
            bool invLoaded = core::StorageManager::loadInventory(inventory, "inventory_save.json");
            bool recLoaded = core::StorageManager::loadRecipes(recipeRepo, "recipes_save.json");

            if (invLoaded || recLoaded) {
                std::cout << "Data successfully loaded from JSON files!\n";
            } else {
                std::cout << "Failed to load data (files may not exist).\n";
            }
        } else if (choice == 15) {
            std::cout << "\n=== Recipe Builder ===\n";

            std::string title;
            std::cout << "Enter recipe title: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, title);

            if (recipeRepo.hasRecipe(title)) {
                std::cout << "Recipe with title '" << title << "' already exists!\n";
            } else {
                int prepTime = 0;
                double calories = 0.0;
                std::string instructions;

                std::cout << "Enter preparation time (minutes): ";
                std::cin >> prepTime;

                calories = readAmount("Enter calories (kcal): ");

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter preparation instructions: ";
                std::getline(std::cin, instructions);

                core::Recipe newRecipe(title, prepTime, calories, instructions);

                bool addingIngredients = true;
                std::cout << "\n=== Adding Ingredients to Recipe ===\n";
                while (addingIngredients) {
                    std::string ingName, unit;

                    std::cout << "Ingredient name: ";
                    std::getline(std::cin, ingName);

                    double amount = readAmount("Amount required: ");

                    std::cout << "Unit (g/ml): ";
                    std::cin >> unit;

                    newRecipe.addRequirement(ingName, amount, unit);

                    while (true) {
                        std::string response;
                        std::cout << "Add another ingredient? (yes/no): ";
                        std::cin >> response;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        if (response == "yes" || response == "y" || response == "Yes" || response == "YES") {
                            addingIngredients = true;
                            break;
                        }
                        if (response == "no" || response == "n" || response == "No" || response == "NO") {
                            addingIngredients = false;
                            break;
                        }
                        std::cout << "Invalid input! Please enter 'yes' or 'no'.\n";
                    }
                }
                recipeRepo.addRecipe(newRecipe);
                std::cout << "Recipe '" << title << "' successfully created and added!\n";
            }
        } else if (choice == 16) {
            recipeRepo.printAllRecipes();
        } else if (choice == 17) {
            auto cookable = recipeRepo.getCookableRecipes(inventory);
            if (cookable.empty()) {
                std::cout << "\nNo recipes can be prepared with current inventory ingredients.\n";
            } else {
                std::cout << "\n=== Recipes You Can Cook Right Now ===\n";
                for (const auto &recipe: cookable) {
                    std::cout << "- " << recipe.getTitle()
                            << " (" << recipe.getPrepTime() << " mins, "
                            << recipe.getCalories() << " kcal)\n";
                }
            }
        } else if (choice == 18) {
            std::cout << "Exiting NutriMesh. Goodbye!\n";
            running = false;
        } else {
            std::cout << "Invalid option! Please choose between 1 and 18.\n";
        }
    }
    return 0;
}
