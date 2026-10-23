#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/AddIngredientDialog.h"
#include "core/StorageManager.h"
#include "core/DateUtils.h"
#include <QHeaderView>
#include <QColor>
#include <QMessageBox>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshInventoryClicked);
    connect(ui->addIngredientButton, &QPushButton::clicked, this, &MainWindow::onAddIngredientClicked);

    connect(ui->recipesListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onRecipeSelectionChanged);

    setupInventoryTable();
    loadData();
    updateInventoryTable();
    updateRecipesList();
}

MainWindow::~MainWindow() {
    delete ui;
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
    const auto &recipes = recipeRepo.getRecipes();

    for (const auto &recipe: recipes) {
        ui->recipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
    }
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
            QString html = QString("<h2>%1</h2>").arg(QString::fromStdString(recipe.getTitle()));
            html += QString("<p><b>⏱️ Prep Time:</b> %1 mins | <b>🔥 Calories:</b> %2 kcal</p>")
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

            ui->recipeDetailsTextEdit->setHtml(html);
            break;
        }
    }
}

void MainWindow::onRefreshInventoryClicked() {
    loadData();
    updateInventoryTable();
    updateRecipesList();
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
