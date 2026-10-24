#include "ui/AddRecipeDialog.h"
#include "ui_AddRecipeDialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>

AddRecipeDialog::AddRecipeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddRecipeDialog), createdRecipe("") {
    ui->setupUi(this);

    ui->requirementsTable->setColumnCount(3);
    ui->requirementsTable->setHorizontalHeaderLabels({"Ingredient Name", "Amount", "Unit"});
    ui->requirementsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->addReqButton->setStyleSheet("background-color: #2b5b84; color: white; border-radius: 6px; padding: 6px 14px;");
    ui->removeReqButton->setStyleSheet(
        "background-color: #6b2e2e; color: white; border-radius: 6px; padding: 6px 14px;");
    ui->saveButton->setStyleSheet(
        "background-color: #4a4a4a; color: white; border-radius: 6px; padding: 6px 14px; font-weight: 500;");
    ui->cancelButton->setStyleSheet(
        "background-color: #3a3a3a; color: white; border-radius: 6px; padding: 6px 14px; font-weight: 500;");

    connect(ui->addReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onAddRequirementClicked);
    connect(ui->removeReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onRemoveRequirementClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &AddRecipeDialog::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddRecipeDialog::~AddRecipeDialog() {
    delete ui;
}

void AddRecipeDialog::onAddRequirementClicked() {
    QString name = ui->reqNameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Ingredient name cannot be empty!");
        return;
    }

    double amount = ui->reqAmountSpinBox->value();
    QString unit = ui->reqUnitComboBox->currentText();

    int row = ui->requirementsTable->rowCount();
    ui->requirementsTable->insertRow(row);
    ui->requirementsTable->setItem(row, 0, new QTableWidgetItem(name));
    ui->requirementsTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
    ui->requirementsTable->setItem(row, 2, new QTableWidgetItem(unit));

    ui->reqNameEdit->clear();
    ui->reqAmountSpinBox->setValue(100);
}

void AddRecipeDialog::onRemoveRequirementClicked() {
    int currentRow = ui->requirementsTable->currentRow();
    if (currentRow >= 0) {
        ui->requirementsTable->removeRow(currentRow);
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select an ingredient to remove!");
    }
}

void AddRecipeDialog::onSaveClicked() {
    QString title = ui->titleLineEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Recipe title cannot be empty!");
        return;
    }

    if (ui->requirementsTable->rowCount() == 0) {
        QMessageBox::warning(this, "Validation Error", "Please add at least one required ingredient!");
        return;
    }

    createdRecipe = core::Recipe(title.toStdString(),
                                 static_cast<size_t>(ui->prepTimeSpinBox->value()),
                                 ui->caloriesSpinBox->value(),
                                 ui->instructionsTextEdit->toPlainText().toStdString());

    for (int i = 0; i < ui->requirementsTable->rowCount(); ++i) {
        std::string reqName = ui->requirementsTable->item(i, 0)->text().toStdString();
        double reqAmount = ui->requirementsTable->item(i, 1)->text().toDouble();
        std::string reqUnit = ui->requirementsTable->item(i, 2)->text().toStdString();

        createdRecipe.addRequirement(reqName, reqAmount, reqUnit);
    }

    accept();
}

core::Recipe AddRecipeDialog::getCreatedRecipe() const {
    return createdRecipe;
}
