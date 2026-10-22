#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "core/StorageManager.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setupInventoryTable();
    loadData();
    updateInventoryTable();
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

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ui->inventoryTable->insertRow(i);
        ui->inventoryTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(items[i]->getName())));
        ui->inventoryTable->setItem(i, 1, new QTableWidgetItem(QString::number(items[i]->getAmount())));
        ui->inventoryTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(items[i]->getUnit())));
        ui->inventoryTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(items[i]->getExpirationDate())));
    }
}

void MainWindow::onRefreshInventoryClicked() {
    loadData();
    updateInventoryTable();
}