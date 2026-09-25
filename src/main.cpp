#include <iostream>
#include <vector>
#include <memory>
#include <core/SolidIngredient.h>
#include <core/LiquidIngredient.h>

int main() {

    // First polimorphysm test
    std::vector<std::unique_ptr<core::Ingredient> > ingredients;

    ingredients.push_back(std::make_unique<core::SolidIngredient>("Flour", 500.0, "2026-12-31"));
    ingredients.push_back(std::make_unique<core::LiquidIngredient>("Milk", 1000.0, "2026-10-15"));


    std::cout << "==========NutriMesh==========\n";
    for (const auto& item : ingredients) {
        item->printInfo();
    }
    return 0;
}
