#include "ui/AddIngredientDialog.h"
#include "ui_AddIngredientDialog.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include "core/CountableIngredient.h"
#include <QMessageBox>
#include <QDate>

AddIngredientDialog::AddIngredientDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddIngredientDialog) {
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    ui->typeComboBox->addItem("Solid (g)", "Solid");
    ui->typeComboBox->addItem("Liquid (ml)", "Liquid");
    ui->typeComboBox->addItem("Countable (pcs)", "Countable");

    connect(ui->typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddIngredientDialog::onTypeChanged);

    connect(ui->addButton, &QPushButton::clicked, this, &AddIngredientDialog::onAddClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    onTypeChanged(0);
}

AddIngredientDialog::~AddIngredientDialog() {
    delete ui;
}

void AddIngredientDialog::onTypeChanged(int index) {
    QString type = ui->typeComboBox->itemData(index).toString();
    if (type == "Solid") {
        ui->unitLabel->setText("g");
    } else if (type == "Liquid") {
        ui->unitLabel->setText("ml");
    } else {
        ui->unitLabel->setText("pcs");
    }
}

void AddIngredientDialog::onAddClicked() {
    QString name = ui->nameLineEdit->text().trimmed();
    double amount = ui->amountSpinBox->value();
    QString expDate = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString type = ui->typeComboBox->currentData().toString();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter ingredient name!");
        return;
    }

    if (amount <= 0) {
        QMessageBox::warning(this, "Input Error", "Amount must be greater than zero!");
        return;
    }

    std::string nameStd = name.toStdString();
    std::string expDateStd = expDate.toStdString();

    if (type == "Solid") {
        createdIngredient = std::make_unique<core::SolidIngredient>(nameStd, amount, expDateStd);
    } else if (type == "Liquid") {
        createdIngredient = std::make_unique<core::LiquidIngredient>(nameStd, amount, expDateStd);
    } else {
        createdIngredient = std::make_unique<core::CountableIngredient>(nameStd, amount, expDateStd);
    }

    accept();
}

std::unique_ptr<core::Ingredient> AddIngredientDialog::getCreatedIngredient() {
    return std::move(createdIngredient);
}
