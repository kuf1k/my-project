#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/AddIngredientDialog.h"
#include "ui/AddRecipeDialog.h"
#include "core/StorageManager.h"
#include "core/DateUtils.h"
#include "core/FileExporter.h"
#include "core/DataFilter.h"
#include <QHeaderView>
#include <QColor>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QComboBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDate>
#include <QTime>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setupSortComboBoxes();
    setupMealPlannerControls();
    setupSmartFilterControls();

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshInventoryClicked);
    connect(ui->addIngredientButton, &QPushButton::clicked, this, &MainWindow::onAddIngredientClicked);
    connect(ui->deleteIngredientButton, &QPushButton::clicked, this, &MainWindow::onDeleteIngredientClicked);
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
    connect(ui->addRecipeButton, &QPushButton::clicked, this, &MainWindow::onAddRecipeClicked);

    connect(ui->addMealPlanButton, &QPushButton::clicked, this, &MainWindow::onAddMealPlanClicked);
    connect(ui->removeMealPlanButton, &QPushButton::clicked, this, &MainWindow::onRemoveMealPlanClicked);
    connect(ui->clearMealPlanButton, &QPushButton::clicked, this, &MainWindow::onClearMealPlanClicked);
    connect(ui->generateWeeklyShoppingButton, &QPushButton::clicked, this,
            &MainWindow::onGenerateWeeklyShoppingClicked);

    connect(ui->applySmartFilterButton, &QPushButton::clicked, this, &MainWindow::onApplySmartFilterClicked);
    connect(ui->resetSmartFilterButton, &QPushButton::clicked, this, &MainWindow::onResetSmartFilterClicked);
    connect(ui->filteredRecipesListWidget, &QListWidget::itemSelectionChanged, this,
            &MainWindow::onFilteredRecipeSelectionChanged);
    connect(ui->quickAddToMealPlanButton, &QPushButton::clicked, this, &MainWindow::onQuickAddToMealPlanClicked);

    setupInventoryTable();
    setupShoppingTable();
    setupMealPlanTable();
    loadData();
    updateInventoryTable();
    updateRecipesList();
    updateShoppingRecipeComboBox();
    updateMealPlanTable();
    onApplySmartFilterClicked();
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

void MainWindow::setupMealPlannerControls() {
    QStringList days = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    ui->mealDayComboBox->clear();
    ui->mealDayComboBox->addItems(days);
    ui->mealTimeEdit->setTime(QTime(10, 0));
}

void MainWindow::setupSmartFilterControls() {
    QStringList days = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    ui->quickScheduleDayComboBox->clear();
    ui->quickScheduleDayComboBox->addItems(days);
    ui->quickScheduleTimeEdit->setTime(QTime(12, 0));
}

void MainWindow::setupShoppingTable() {
    ui->shoppingTableWidget->setColumnCount(3);
    QStringList headers = {"Ingredient Name", "Amount Needed to Buy", "Unit"};
    ui->shoppingTableWidget->setHorizontalHeaderLabels(headers);
    ui->shoppingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setupMealPlanTable() {
    ui->mealPlanTableWidget->setColumnCount(5);
    QStringList headers = {"Day", "Time", "Recipe", "Prep Time (mins)", "Calories (kcal)"};
    ui->mealPlanTableWidget->setHorizontalHeaderLabels(headers);
    ui->mealPlanTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->mealPlanTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mealPlanTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->mealPlanTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateShoppingRecipeComboBox() {
    ui->shoppingRecipeComboBox->clear();
    ui->mealRecipeComboBox->clear();
    const auto &recipes = recipeRepo.getRecipes();
    for (const auto &recipe: recipes) {
        QString title = QString::fromStdString(recipe.getTitle());
        ui->shoppingRecipeComboBox->addItem(title);
        ui->mealRecipeComboBox->addItem(title);
    }
}

void MainWindow::loadData() {
    core::StorageManager::loadInventory(inventory, "inventory_save.json");
    core::StorageManager::loadRecipes(recipeRepo, "recipes_save.json");
    core::StorageManager::loadMealPlan(mealPlanner, "meal_plan_save.json");
}

void MainWindow::setupInventoryTable() {
    ui->inventoryTable->setColumnCount(4);
    QStringList headers = {"Name", "Amount", "Unit", "Expiration Date"};
    ui->inventoryTable->setHorizontalHeaderLabels(headers);
    ui->inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->inventoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->inventoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->inventoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateInventoryTable() {
    ui->inventoryTable->setRowCount(0);
    const auto &items = inventory.getItems();

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();

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

void MainWindow::updateMealPlanTable() {
    ui->mealPlanTableWidget->setRowCount(0);
    const auto &entries = mealPlanner.getEntries();

    for (int i = 0; i < static_cast<int>(entries.size()); ++i) {
        ui->mealPlanTableWidget->insertRow(i);

        QString prepStr = "-";
        QString calStr = "-";

        for (const auto &recipe: recipeRepo.getRecipes()) {
            if (recipe.getTitle() == entries[i].recipeTitle) {
                prepStr = QString::number(recipe.getPrepTime());
                calStr = QString::number(recipe.getCalories());
                break;
            }
        }

        ui->mealPlanTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(entries[i].dayOfWeek)));
        ui->mealPlanTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(entries[i].time)));
        ui->mealPlanTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(entries[i].recipeTitle)));
        ui->mealPlanTableWidget->setItem(i, 3, new QTableWidgetItem(prepStr));
        ui->mealPlanTableWidget->setItem(i, 4, new QTableWidgetItem(calStr));
    }

    double totalCal = mealPlanner.getTotalCalories(recipeRepo);
    ui->mealTotalCaloriesLabel->setText(
        QString("<b>Total Scheduled Calories:</b> %1 kcal").arg(totalCal)
    );
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
        spdlog::info("User checked cookable recipes: None available with current pantry items.");
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
    spdlog::info("User found {} cookable recipe(s).", cookableRecipes.size());
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
        spdlog::info("User cooked recipe: '{}'. Pantry ingredients deducted.", title);
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
        spdlog::warn("User failed to cook recipe: '{}'. Insufficient ingredients.", title);
        QMessageBox::critical(this, "Error",
                              "Not enough ingredients in your pantry to cook this recipe!");
    }
}

void MainWindow::onAddRecipeClicked() {
    AddRecipeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        core::Recipe newRecipe = dialog.getCreatedRecipe();

        if (recipeRepo.hasRecipe(newRecipe.getTitle())) {
            spdlog::warn("Attempted to add duplicate recipe: '{}'", newRecipe.getTitle());
            QMessageBox::warning(this, "Duplicate Recipe",
                                 QString("Recipe '%1' already exists!").arg(
                                     QString::fromStdString(newRecipe.getTitle())));
            return;
        }

        recipeRepo.addRecipe(newRecipe);
        spdlog::info("User added new recipe: '{}' ({} mins, {} kcal)", newRecipe.getTitle(), newRecipe.getPrepTime(),
                     newRecipe.getCalories());
        core::StorageManager::saveRecipes(recipeRepo, "recipes_save.json");

        updateRecipesList();
        updateShoppingRecipeComboBox();

        QMessageBox::information(this, "Success", "Custom recipe added successfully!");
    }
}

void MainWindow::onDeleteIngredientClicked() {
    int selectedRow = ui->inventoryTable->currentRow();
    const auto &items = inventory.getItems();

    if (selectedRow < 0 || selectedRow >= static_cast<int>(items.size())) {
        QMessageBox::warning(this, "No Selection",
                             "Please select an ingredient row in the table first!");
        return;
    }

    QString itemName = QString::fromStdString(items[selectedRow]->getName());
    double itemAmount = items[selectedRow]->getAmount();
    QString itemUnit = QString::fromStdString(items[selectedRow]->getUnit());

    auto reply = QMessageBox::question(
        this,
        "Confirm Deletion",
        QString("Are you sure you want to delete '%1' (%2 %3)?")
            .arg(itemName)
            .arg(itemAmount)
            .arg(itemUnit),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) {
        return;
    }

    if (inventory.removeIngredientAt(static_cast<size_t>(selectedRow))) {
        spdlog::info("User deleted ingredient: '{}' ({:.1f} {})",
                     itemName.toStdString(), itemAmount, itemUnit.toStdString());
        core::StorageManager::saveInventory(inventory, "inventory_save.json");
        updateInventoryTable();
    }
}

void MainWindow::onRemoveExpiredClicked() {
    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    size_t removedCount = inventory.removeExpired(currentDate);

    if (removedCount == 0) {
        spdlog::info("User triggered expired items removal: Pantry is clean.");
        QMessageBox::information(this, "Clean Pantry",
                                 "No expired ingredients found in your pantry!");
        return;
    }

    spdlog::warn("User removed {} expired ingredient(s) from pantry.", removedCount);
    core::StorageManager::saveInventory(inventory, "inventory_save.json");
    updateInventoryTable();

    QMessageBox::information(this, "Expired Removed",
                             QString("Successfully removed %1 expired ingredient(s) from your pantry.")
                             .arg(removedCount));
}

void MainWindow::onExportInventoryClicked() {
    if (inventory.getItemsCount() == 0) {
        spdlog::warn("Attempted to export empty inventory.");
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

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    core::FileExporter exporter("Pantry Inventory Report", fileName.toStdString());

    if (exporter.exportData(inventory, currentDate)) {
        spdlog::info("Exported inventory report to: {}", fileName.toStdString());
        QMessageBox::information(this, "Success",
                                 "Inventory report successfully exported to " + fileName);
    } else {
        spdlog::error("Failed to export inventory report to: {}", fileName.toStdString());
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
        spdlog::info("Generated shopping list for '{}': All ingredients already available.", title);
        QMessageBox::information(this, "Shopping List",
                                 "You already have all the necessary ingredients in your pantry for " + selectedTitle +
                                 "!");
        return;
    }

    spdlog::info("User generated shopping list for '{}' ({} missing item(s)).", title, items.size());

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ui->shoppingTableWidget->insertRow(i);
        ui->shoppingTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(items[i].name)));
        ui->shoppingTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(items[i].amountToBuy)));
        ui->shoppingTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(items[i].unit)));
    }
}

void MainWindow::onExportShoppingListClicked() {
    if (shoppingList.isEmpty()) {
        spdlog::warn("Attempted to export an empty shopping list.");
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
        spdlog::info("Exported shopping list to: {}", fileName.toStdString());
        QMessageBox::information(this, "Success",
                                 "Shopping list successfully exported to " + fileName);
    } else {
        spdlog::error("Failed to export shopping list to: {}", fileName.toStdString());
        QMessageBox::critical(this, "Error", "Failed to export shopping list to file.");
    }
}

void MainWindow::onAddMealPlanClicked() {
    QString day = ui->mealDayComboBox->currentText();
    QString time = ui->mealTimeEdit->time().toString("HH:mm");
    QString recipeTitle = ui->mealRecipeComboBox->currentText();

    if (recipeTitle.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No recipes available! Please add a recipe first.");
        return;
    }

    mealPlanner.addMeal(day.toStdString(), time.toStdString(), recipeTitle.toStdString());
    core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
    updateMealPlanTable();

    spdlog::info("Scheduled meal '{}' on {} at {}", recipeTitle.toStdString(), day.toStdString(), time.toStdString());
}

void MainWindow::onRemoveMealPlanClicked() {
    int selectedRow = ui->mealPlanTableWidget->currentRow();
    if (selectedRow < 0 || selectedRow >= static_cast<int>(mealPlanner.getEntriesCount())) {
        QMessageBox::warning(this, "No Selection", "Please select a scheduled meal row first!");
        return;
    }

    if (mealPlanner.removeMealAt(static_cast<size_t>(selectedRow))) {
        core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
        updateMealPlanTable();
        spdlog::info("Removed scheduled meal at row {}", selectedRow);
    }
}

void MainWindow::onClearMealPlanClicked() {
    if (mealPlanner.getEntriesCount() == 0) {
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Clear Schedule",
        "Are you sure you want to clear the entire weekly meal schedule?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        mealPlanner.clear();
        core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
        updateMealPlanTable();
        spdlog::info("User cleared the weekly meal schedule.");
    }
}

void MainWindow::onGenerateWeeklyShoppingClicked() {
    auto plannedRecipes = mealPlanner.getPlannedRecipes(recipeRepo);
    if (plannedRecipes.empty()) {
        QMessageBox::warning(this, "Empty Schedule",
                             "Your meal schedule is empty! Add some meals first.");
        return;
    }

    shoppingList.generateForRecipes(plannedRecipes, inventory);

    ui->shoppingTableWidget->setRowCount(0);
    const auto &items = shoppingList.getItems();

    ui->tabWidget->setCurrentWidget(ui->tabShopping);

    if (items.empty()) {
        QMessageBox::information(this, "All Set!",
                                 "You already have all the ingredients needed for your entire weekly meal plan!");
        return;
    }

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ui->shoppingTableWidget->insertRow(i);
        ui->shoppingTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(items[i].name)));
        ui->shoppingTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(items[i].amountToBuy)));
        ui->shoppingTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(items[i].unit)));
    }

    spdlog::info("Generated weekly shopping list from {} planned meals ({} missing item(s)).",
                 plannedRecipes.size(), items.size());
    QMessageBox::information(this, "Weekly Shopping List Ready",
                             QString("Generated shopping list for %1 scheduled meal(s)!")
                             .arg(plannedRecipes.size()));
}

void MainWindow::onApplySmartFilterClicked() {
    int maxCalories = ui->filterMaxCaloriesSpinBox->value();
    int maxTime = ui->filterMaxTimeSpinBox->value();
    QString ingredientQuery = ui->filterIngredientLineEdit->text().trimmed().toLower();

    auto filtered = core::DataFilter<core::Recipe>::filterBy(
        recipeRepo.getRecipes(),
        [maxCalories, maxTime, &ingredientQuery](const core::Recipe &recipe) {
            if (maxCalories > 0 && recipe.getCalories() > static_cast<double>(maxCalories)) {
                return false;
            }
            if (maxTime > 0 && recipe.getPrepTime() > static_cast<size_t>(maxTime)) {
                return false;
            }
            if (!ingredientQuery.isEmpty()) {
                bool hasIngredient = core::DataFilter<core::RecipeRequirement>::contains(
                    recipe.getRequirements(),
                    [&ingredientQuery](const core::RecipeRequirement &req) {
                        return QString::fromStdString(req.name).toLower().contains(ingredientQuery);
                    }
                );
                if (!hasIngredient) {
                    return false;
                }
            }
            return true;
        }
    );

    ui->filteredRecipesListWidget->clear();
    ui->filteredRecipeDetailsTextEdit->clear();

    for (const auto &recipe: filtered) {
        ui->filteredRecipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
    }

    spdlog::info("Smart filter applied (maxCal={}, maxTime={}, ingredient='{}'): {} recipe(s) matched.",
                 maxCalories, maxTime, ingredientQuery.toStdString(), filtered.size());
}

void MainWindow::onResetSmartFilterClicked() {
    ui->filterMaxCaloriesSpinBox->setValue(0);
    ui->filterMaxTimeSpinBox->setValue(0);
    ui->filterIngredientLineEdit->clear();
    onApplySmartFilterClicked();
}

void MainWindow::onFilteredRecipeSelectionChanged() {
    QListWidgetItem *selectedItem = ui->filteredRecipesListWidget->currentItem();
    if (!selectedItem) {
        ui->filteredRecipeDetailsTextEdit->clear();
        return;
    }

    std::string title = selectedItem->text().toStdString();
    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            displayRecipeDetails(recipe, ui->filteredRecipeDetailsTextEdit);
            break;
        }
    }
}

void MainWindow::onQuickAddToMealPlanClicked() {
    QListWidgetItem *selectedItem = ui->filteredRecipesListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "No Selection", "Please select a recipe from the filtered list first!");
        return;
    }

    QString day = ui->quickScheduleDayComboBox->currentText();
    QString time = ui->quickScheduleTimeEdit->time().toString("HH:mm");
    QString recipeTitle = selectedItem->text();

    mealPlanner.addMeal(day.toStdString(), time.toStdString(), recipeTitle.toStdString());
    core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
    updateMealPlanTable();

    spdlog::info("Quick-scheduled filtered recipe '{}' on {} at {}",
                 recipeTitle.toStdString(), day.toStdString(), time.toStdString());

    QMessageBox::information(this, "Added to Schedule",
                             QString("'%1' scheduled for %2 at %3!")
                             .arg(recipeTitle, day, time));
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
    spdlog::info("User refreshed inventory data from files.");
    loadData();
    updateInventoryTable();
    updateRecipesList();
    updateShoppingRecipeComboBox();
    updateMealPlanTable();
    onApplySmartFilterClicked();
}

void MainWindow::onAddIngredientClicked() {
    AddIngredientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto newIngredient = dialog.getCreatedIngredient();
        if (newIngredient) {
            spdlog::info("User added ingredient: '{}' ({:.1f} {})", newIngredient->getName(),
                         newIngredient->getAmount(), newIngredient->getUnit());
            inventory.addIngredient(std::move(newIngredient));
            core::StorageManager::saveInventory(inventory, "inventory_save.json");
            updateInventoryTable();
            QMessageBox::information(this, "Success", "Ingredient added successfully!");
        }
    }
}
