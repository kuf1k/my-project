#include "ui/AddRecipeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

AddRecipeDialog::AddRecipeDialog(QWidget *parent)
    : QDialog(parent), createdRecipe("") {
    setWindowTitle("➕ Add Custom Recipe");
    resize(500, 550);
    setupUi();


}

void AddRecipeDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QFormLayout();

    titleLineEdit = new QLineEdit(this);
    titleLineEdit->setPlaceholderText("e.g. Homemade Pizza");

    prepTimeSpinBox = new QSpinBox(this);
    prepTimeSpinBox->setRange(1, 1440);
    prepTimeSpinBox->setValue(15);
    prepTimeSpinBox->setSuffix(" mins");

    caloriesSpinBox = new QDoubleSpinBox(this);
    caloriesSpinBox->setRange(0, 10000);
    caloriesSpinBox->setValue(350);
    caloriesSpinBox->setSuffix(" kcal");

    instructionsTextEdit = new QTextEdit(this);
    instructionsTextEdit->setPlaceholderText("Enter cooking steps...");
    instructionsTextEdit->setMaximumHeight(100);

    formLayout->addRow("Recipe Title:", titleLineEdit);
    formLayout->addRow("Prep Time:", prepTimeSpinBox);
    formLayout->addRow("Calories:", caloriesSpinBox);
    formLayout->addRow("Instructions:", instructionsTextEdit);

    mainLayout->addLayout(formLayout);

    mainLayout->addWidget(new QLabel("<b>🛒 Required Ingredients:</b>", this));

    auto *reqInputLayout = new QHBoxLayout();
    reqNameEdit = new QLineEdit(this);
    reqNameEdit->setPlaceholderText("Ingredient name (e.g. Flour)");

    reqAmountSpinBox = new QDoubleSpinBox(this);
    reqAmountSpinBox->setRange(0.1, 10000);
    reqAmountSpinBox->setValue(100);

    reqUnitComboBox = new QComboBox(this);
    reqUnitComboBox->addItems({"g", "ml", "pcs"});

    addReqButton = new QPushButton("➕ Add Ingredient", this);
    addReqButton->setStyleSheet("background-color: #2b5b84; color: white; border-radius: 6px; padding: 6px 14px;");

    reqInputLayout->addWidget(reqNameEdit, 2);
    reqInputLayout->addWidget(reqAmountSpinBox, 1);
    reqInputLayout->addWidget(reqUnitComboBox, 1);
    reqInputLayout->addWidget(addReqButton, 1);

    mainLayout->addLayout(reqInputLayout);

    requirementsTable = new QTableWidget(0, 3, this);
    requirementsTable->setHorizontalHeaderLabels({"Ingredient Name", "Amount", "Unit"});
    requirementsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(requirementsTable);

    removeReqButton = new QPushButton("🗑️ Remove Selected Ingredient", this);
    removeReqButton->setStyleSheet("background-color: #6b2e2e; color: white; border-radius: 6px; padding: 6px 14px;");
    mainLayout->addWidget(removeReqButton);

    auto *buttonLayout = new QHBoxLayout();
    auto *saveButton = new QPushButton("💾 Save Recipe", this);
    auto *cancelButton = new QPushButton("Cancel", this);

    saveButton->setObjectName("primaryButton");
    cancelButton->setObjectName("cancelButton");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    connect(addReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onAddRequirementClicked);
    connect(removeReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onRemoveRequirementClicked);
    connect(saveButton, &QPushButton::clicked, this, &AddRecipeDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void AddRecipeDialog::onAddRequirementClicked() {
    QString name = reqNameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Ingredient name cannot be empty!");
        return;
    }

    double amount = reqAmountSpinBox->value();
    QString unit = reqUnitComboBox->currentText();

    int row = requirementsTable->rowCount();
    requirementsTable->insertRow(row);
    requirementsTable->setItem(row, 0, new QTableWidgetItem(name));
    requirementsTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
    requirementsTable->setItem(row, 2, new QTableWidgetItem(unit));

    reqNameEdit->clear();
    reqAmountSpinBox->setValue(100);
}

void AddRecipeDialog::onRemoveRequirementClicked() {
    int currentRow = requirementsTable->currentRow();
    if (currentRow >= 0) {
        requirementsTable->removeRow(currentRow);
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select an ingredient to remove!");
    }
}

void AddRecipeDialog::onSaveClicked() {
    QString title = titleLineEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Recipe title cannot be empty!");
        return;
    }

    if (requirementsTable->rowCount() == 0) {
        QMessageBox::warning(this, "Validation Error", "Please add at least one required ingredient!");
        return;
    }

    createdRecipe = core::Recipe(title.toStdString(),
                                 static_cast<size_t>(prepTimeSpinBox->value()),
                                 caloriesSpinBox->value(),
                                 instructionsTextEdit->toPlainText().toStdString());

    for (int i = 0; i < requirementsTable->rowCount(); ++i) {
        std::string reqName = requirementsTable->item(i, 0)->text().toStdString();
        double reqAmount = requirementsTable->item(i, 1)->text().toDouble();
        std::string reqUnit = requirementsTable->item(i, 2)->text().toStdString();

        createdRecipe.addRequirement(reqName, reqAmount, reqUnit);
    }

    accept();
}

core::Recipe AddRecipeDialog::getCreatedRecipe() const {
    return createdRecipe;
}
