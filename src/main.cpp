#include <iostream>
#include <memory>
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include "core/Inventory.h"


void displayMenu() {
    std::cout << "\n=================================\n";
    std::cout << "     NutriMesh - Kitchen Assistant \n";
    std::cout << "=================================\n";
    std::cout << "1. Add Solid Ingredient (g)\n";
    std::cout << "2. Add Liquid Ingredient (ml)\n";
    std::cout << "3. View Inventory\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option (1-4): ";
}

int main() {
    core::Inventory inventory;
    bool running = true;

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
            double amount;

            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter amount (g): ";
            std::cin >> amount;
            std::cout << "Enter expiration date (YYYY-MM-DD): ";
            std::cin >> expirationDate;

            auto item = std::make_unique<core::SolidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Solid ingredient has been successfully added!\n";
        }
        else if (choice == 2) {
            std::string name, expirationDate;
            double amount;

            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter amount (ml): ";
            std::cin >> amount;
            std::cout << "Enter expiration date (YYYY-MM-DD): ";
            std::cin >> expirationDate;

            auto item = std::make_unique<core::LiquidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Liquid ingredient has been successfully added!\n";
        }
        else if (choice == 3) {
            inventory.printInventory();
        }
        else if (choice == 4) {
            std::cout << "Exiting NutriMesh. Goodbye!\n";
            running = false;
        }
        else {
            std::cout << "Invalid option! Please choose between 1 and 4.\n";
        }
    }

    return 0;
}
