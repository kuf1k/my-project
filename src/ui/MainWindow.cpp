#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/AddIngredientDialog.h"
#include "core/StorageManager.h"
#include "core/DateUtils.h"
#include "core/FileExporter.h"
#include <QHeaderView>
#include <QColor>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QComboBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setupSortComboBoxes();

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshInventoryClicked);
    connect(ui->addIngredientButton, &QPushButton::clicked, this, &MainWindow::onAddIngredientClicked);
    connect(ui->removeExpiredButton, &QPushButton::clicked, this, &MainWindow::onRemoveExpiredClicked);
    connect(ui->exportInventoryButton, &QPushButton::clicked, this, &MainWindow::onExportInventoryClicked);
    connect(ui->recipesListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onRecipeSelectionChanged);

    connect(ui->findRecipesButton, &QPushButton::clicked, this, &MainWindow::onFindCookableRecipesClicked);
    connect(ui->cookableRecipesListWidget, &QListWidget::itemSelectionChanged, this,
            &MainWindow::onCookableRecipeSelectionChanged);

    connect(ui->recipesSortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onRecipesSortChanged);
    connect(ui->matcherSortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onMatcherSortChanged);

    connect(ui->generateShoppingListButton, &QPushButton::clicked, this, &MainWindow::onGenerateShoppingListClicked);
    connect(ui->exportShoppingListButton, &QPushButton::clicked, this, &MainWindow::onExportShoppingListClicked);

    connect(ui->cookRecipeButton, &QPushButton::clicked, this, &MainWindow::onCookRecipeClicked);

    setupInventoryTable();
    setupShoppingTable();
    loadData();
    updateInventoryTable();
    updateRecipesList();
    updateShoppingRecipeComboBox();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupSortComboBoxes() {
    QStringList options = {
        "By Name (A-Z)",
        "By Prep Time (Fastest First)",
        "By Calories (Low to High)",
        "By Calories (High to Low)"
    };
    ui->recipesSortComboBox->clear();
    ui->recipesSortComboBox->addItems(options);

    ui->matcherSortComboBox->clear();
    ui->matcherSortComboBox->addItems(options);
}

void MainWindow::setupShoppingTable() {
    ui->shoppingTableWidget->setColumnCount(3);
    QStringList headers = {"Ingredient Name", "Amount Needed to Buy", "Unit"};
    ui->shoppingTableWidget->setHorizontalHeaderLabels(headers);
    ui->shoppingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::updateShoppingRecipeComboBox() {
    ui->shoppingRecipeComboBox->clear();
    const auto &recipes = recipeRepo.getRecipes();
    for (const auto &recipe: recipes) {
        ui->shoppingRecipeComboBox->addItem(QString::fromStdString(recipe.getTitle()));
    }
}

void MainWindow::loadData() {
    core::StorageManager::loadInventory(inventory, "inventory_save.json");
    core::StorageManager::loadRecipes(recipeRepo, "recipes_save.json");
}

void MainWindow::setupInventoryTable() {
    ui->inventoryTable->setColumnCount(4);
    QStringList headers = {"Name", "Amount", "Unit", "Expiration Date"};
    ui->inventoryTable->setHorizontalHeaderLabels(headers);
    ui->inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::updateInventoryTable() {
    ui->inventoryTable->setRowCount(0);
    const auto &items = inventory.getItems();

    std::string currentDate = "2026-10-23";

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ui->inventoryTable->insertRow(i);

        auto *nameItem = new QTableWidgetItem(QString::fromStdString(items[i]->getName()));
        auto *amountItem = new QTableWidgetItem(QString::number(items[i]->getAmount()));
        auto *unitItem = new QTableWidgetItem(QString::fromStdString(items[i]->getUnit()));
        auto *expItem = new QTableWidgetItem(QString::fromStdString(items[i]->getExpirationDate()));

        int daysLeft = core::DateUtils::getDaysUntil(items[i]->getExpirationDate(), currentDate);

        QColor rowColor;
        if (daysLeft < 0) {
            rowColor = QColor(120, 40, 40);
        } else if (daysLeft <= 3) {
            rowColor = QColor(110, 90, 30);
        } else {
            rowColor = QColor(35, 45, 35);
        }
        nameItem->setBackground(rowColor);
        amountItem->setBackground(rowColor);
        unitItem->setBackground(rowColor);
        expItem->setBackground(rowColor);

        ui->inventoryTable->setItem(i, 0, nameItem);
        ui->inventoryTable->setItem(i, 1, amountItem);
        ui->inventoryTable->setItem(i, 2, unitItem);
        ui->inventoryTable->setItem(i, 3, expItem);
    }
}

void MainWindow::updateRecipesList() {
    ui->recipesListWidget->clear();

    int currentIndex = ui->recipesSortComboBox->currentIndex();
    if (currentIndex < 0) currentIndex = 0;
    auto sortOption = static_cast<core::RecipeSortOption>(currentIndex);

    auto sortedRecipes = recipeRepo.getSortedRecipes(sortOption);

    for (const auto &recipe: sortedRecipes) {
        ui->recipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
    }
}

void MainWindow::displayRecipeDetails(const core::Recipe &recipe, QTextEdit *targetTextEdit) {
    if (!targetTextEdit) return;

    QString html = QString("<h2>%1</h2>").arg(QString::fromStdString(recipe.getTitle()));
    html += QString("<p style='line-height: 1.4;'><b>⏱️ Prep Time:</b> %1 mins<br><b>🔥 Calories:</b> %2 kcal</p>")
            .arg(recipe.getPrepTime())
            .arg(recipe.getCalories());

    html += "<h3>🛒 Required Ingredients:</h3><ul>";
    for (const auto &req: recipe.getRequirements()) {
        html += QString("<li><b>%1</b>: %2 %3</li>")
                .arg(QString::fromStdString(req.name))
                .arg(req.requiredAmount)
                .arg(QString::fromStdString(req.unit));
    }
    html += "</ul>";

    html += "<h3>📝 Instructions:</h3>";
    QString rawInstructions = QString::fromStdString(recipe.getInstructions());

    if (rawInstructions.contains(';')) {
        QStringList steps = rawInstructions.split(';', Qt::SkipEmptyParts);
        html += "<ol>";
        for (QString step: steps) {
            step = step.trimmed();
            step.remove(QRegularExpression("^\\d+\\)\\s*"));
            if (!step.isEmpty()) {
                html += QString("<li>%1</li>").arg(step);
            }
        }
        html += "</ol>";
    } else {
        html += QString("<p>%1</p>").arg(rawInstructions);
    }

    targetTextEdit->setHtml(html);
}

void MainWindow::onRecipeSelectionChanged() {
    QListWidgetItem *selectedItem = ui->recipesListWidget->currentItem();
    if (!selectedItem) {
        ui->recipeDetailsTextEdit->clear();
        return;
    }

    std::string title = selectedItem->text().toStdString();
    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            displayRecipeDetails(recipe, ui->recipeDetailsTextEdit);
            break;
        }
    }
}

void MainWindow::onFindCookableRecipesClicked() {
    ui->cookableRecipesListWidget->clear();
    ui->cookableRecipeDetailsTextEdit->clear();

    auto cookableRecipes = recipeRepo.getCookableRecipes(inventory);

    if (cookableRecipes.empty()) {
        QMessageBox::information(this, "No Matching Recipes",
                                 "Sorry, you don't have enough ingredients in your pantry for any recipe right now!");
        return;
    }

    int currentIndex = ui->matcherSortComboBox->currentIndex();
    if (currentIndex < 0) currentIndex = 0;
    auto sortOption = static_cast<core::RecipeSortOption>(currentIndex);

    cookableRecipes = core::RecipeRepository::sortRecipes(cookableRecipes, sortOption);

    for (const auto &recipe: cookableRecipes) {
        ui->cookableRecipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
    }
}

void MainWindow::onCookableRecipeSelectionChanged() {
    QListWidgetItem *selectedItem = ui->cookableRecipesListWidget->currentItem();
    if (!selectedItem) {
        ui->cookableRecipeDetailsTextEdit->clear();
        return;
    }

    std::string title = selectedItem->text().toStdString();
    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            displayRecipeDetails(recipe, ui->cookableRecipeDetailsTextEdit);
            break;
        }
    }
}

void MainWindow::onCookRecipeClicked() {
    QListWidgetItem *selectedItem = ui->cookableRecipesListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Select Recipe", "Please select a cookable recipe first!");
        return;
    }

    std::string title = selectedItem->text().toStdString();
    const core::Recipe *targetRecipe = nullptr;

    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            targetRecipe = &recipe;
            break;
        }
    }

    if (!targetRecipe) return;

    if (inventory.consumeRecipe(*targetRecipe)) {
        core::StorageManager::saveInventory(inventory, "inventory_save.json");
        updateInventoryTable();

        ui->cookableRecipesListWidget->clear();
        ui->cookableRecipeDetailsTextEdit->clear();

        auto cookableRecipes = recipeRepo.getCookableRecipes(inventory);
        if (!cookableRecipes.empty()) {
            int currentIndex = ui->matcherSortComboBox->currentIndex();
            if (currentIndex < 0) currentIndex = 0;
            auto sortOption = static_cast<core::RecipeSortOption>(currentIndex);
            cookableRecipes = core::RecipeRepository::sortRecipes(cookableRecipes, sortOption);

            for (const auto &recipe: cookableRecipes) {
                ui->cookableRecipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
            }
        }

        QMessageBox::information(this, "Bon Appétit! 🍽️",
                                 QString(
                                     "You successfully cooked '%1'! Required ingredients have been deducted from your pantry.")
                                 .arg(QString::fromStdString(title)));
    } else {
        QMessageBox::critical(this, "Error",
                              "Not enough ingredients in your pantry to cook this recipe!");
    }
}

void MainWindow::onRemoveExpiredClicked() {
    std::string currentDate = "2026-10-23";
    size_t removedCount = inventory.removeExpired(currentDate);

    if (removedCount == 0) {
        QMessageBox::information(this, "Clean Pantry",
                                 "No expired ingredients found in your pantry!");
        return;
    }

    core::StorageManager::saveInventory(inventory, "inventory_save.json");
    updateInventoryTable();

    QMessageBox::information(this, "Expired Removed",
                             QString("Successfully removed %1 expired ingredient(s) from your pantry.")
                             .arg(removedCount));
}

void MainWindow::onExportInventoryClicked() {
    if (inventory.getItemsCount() == 0) {
        QMessageBox::warning(this, "Export Warning", "Your pantry is empty! Nothing to export.");
        return;
    }

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                          + "/inventory_report.txt";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Export Inventory Report",
                                                    defaultPath,
                                                    "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    std::string currentDate = "2026-10-23";
    core::FileExporter exporter("Pantry Inventory Report", fileName.toStdString());

    if (exporter.exportData(inventory, currentDate)) {
        QMessageBox::information(this, "Success",
                                 "Inventory report successfully exported to " + fileName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to export inventory report to file.");
    }
}

void MainWindow::onGenerateShoppingListClicked() {
    QString selectedTitle = ui->shoppingRecipeComboBox->currentText();
    if (selectedTitle.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a recipe first!");
        return;
    }

    std::string title = selectedTitle.toStdString();
    bool found = false;

    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            shoppingList.generateForRecipe(recipe, inventory);
            found = true;
            break;
        }
    }

    if (!found) return;

    ui->shoppingTableWidget->setRowCount(0);
    const auto &items = shoppingList.getItems();

    if (items.empty()) {
        QMessageBox::information(this, "Shopping List",
                                 "You already have all the necessary ingredients in your pantry for " + selectedTitle +
                                 "!");
        return;
    }

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ui->shoppingTableWidget->insertRow(i);
        ui->shoppingTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(items[i].name)));
        ui->shoppingTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(items[i].amountToBuy)));
        ui->shoppingTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(items[i].unit)));
    }
}

void MainWindow::onExportShoppingListClicked() {
    if (shoppingList.isEmpty()) {
        QMessageBox::warning(this, "Export Warning",
                             "The shopping list is empty! Generate a shopping list first.");
        return;
    }

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                          + "/shopping_list.txt";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Export Shopping List",
                                                    defaultPath,
                                                    "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    if (core::FileExporter::exportShoppingListToTxt(shoppingList, fileName.toStdString())) {
        QMessageBox::information(this, "Success",
                                 "Shopping list successfully exported to " + fileName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to export shopping list to file.");
    }
}

void MainWindow::onRecipesSortChanged(int index) {
    Q_UNUSED(index);
    updateRecipesList();
}

void MainWindow::onMatcherSortChanged(int index) {
    Q_UNUSED(index);
    onFindCookableRecipesClicked();
}

void MainWindow::onRefreshInventoryClicked() {
    loadData();
    updateInventoryTable();
    updateRecipesList();
    updateShoppingRecipeComboBox();
}

void MainWindow::onAddIngredientClicked() {
    AddIngredientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto newIngredient = dialog.getCreatedIngredient();
        if (newIngredient) {
            inventory.addIngredient(std::move(newIngredient));
            core::StorageManager::saveInventory(inventory, "inventory_save.json");
            updateInventoryTable();
            QMessageBox::information(this, "Success", "Ingredient added successfully!");
        }
    }
}
