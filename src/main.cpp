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
double readAmount(const std::string& inputText) {
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

std::string readDate(const std::string& inputText) {
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

void enterInfo (std::string& name, double& amount, std::string& expirationDate) {
    std::cout << "Enter name: ";
    std::cin >> name;
    amount = readAmount("Enter amount: ");
    expirationDate = readDate("Enter expiration date (YYYY-MM-DD): ");
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
            double amount = 0;

            enterInfo(name, amount, expirationDate);

            auto item = std::make_unique<core::SolidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Solid ingredient has been successfully added!\n";
        }
        else if (choice == 2) {
            std::string name, expirationDate;
            double amount = 0;

           enterInfo(name, amount, expirationDate);

            auto item = std::make_unique<core::LiquidIngredient>(name, amount, expirationDate);
            inventory.addIngredient(std::move(item));
            std::cout << "Liquid ingredient has been successfully added!\n";
        }
        else if (choice == 3) {
            std::string currentDate = readDate("Enter current date (YYYY-MM-DD): ");
            inventory.printInventory(currentDate);
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
