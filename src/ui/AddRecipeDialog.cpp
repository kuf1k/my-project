#include "ui/AddRecipeDialog.h"
#include "ui_AddRecipeDialog.h"
#include "ui/TranslationManager.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QCompleter>
#include <QStringList>
#include <spdlog/spdlog.h>

AddRecipeDialog::AddRecipeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddRecipeDialog), createdRecipe("") {
    ui->setupUi(this);

    auto &tm = TranslationManager::instance();
    bool isUk = (tm.getCurrentLanguage() == Language::Ukrainian);

    setWindowTitle(isUk ? "➕ Додати власний рецепт" : "➕ Add Custom Recipe");
    ui->labelTitle->setText(tm.tr("lbl_title"));
    ui->labelPrepTime->setText(isUk ? "Час готування:" : "Prep Time:");
    ui->labelCalories->setText(isUk ? "Калорійність:" : "Calories:");
    ui->labelInstructions->setText(tm.tr("lbl_instructions"));
    ui->labelReqTitle->setText(
        isUk ? "<b>🛒 Необхідні інгредієнти:</b>" : "<b>🛒 Required Ingredients:</b>"
    );

    ui->titleLineEdit->setPlaceholderText(isUk ? "напр. Homemade Pizza" : "e.g. Homemade Pizza");
    ui->prepTimeSpinBox->setSuffix(isUk ? " хв" : " mins");
    ui->caloriesSpinBox->setSuffix(isUk ? " ккал" : " kcal");
    ui->instructionsTextEdit->setPlaceholderText(
        isUk ? "Введіть кроки приготування..." : "Enter cooking steps..."
    );
    ui->reqNameEdit->setPlaceholderText(
        isUk ? "Назва інгредієнта (напр. Flour)" : "Ingredient name (e.g. Flour)"
    );

    ui->addReqButton->setText(tm.tr("btn_add_req"));
    ui->removeReqButton->setText(
        isUk ? "🗑️ Видалити вибраний інгредієнт" : "🗑️ Remove Selected Ingredient"
    );
    ui->cancelButton->setText(tm.tr("btn_cancel"));
    ui->saveButton->setText(tm.tr("btn_save"));

    ui->reqUnitComboBox->clear();
    ui->reqUnitComboBox->addItem(tm.translateUnit("g"), "g");
    ui->reqUnitComboBox->addItem(tm.translateUnit("ml"), "ml");
    ui->reqUnitComboBox->addItem(tm.translateUnit("pcs"), "pcs");

    ui->requirementsTable->setColumnCount(3);
    ui->requirementsTable->setHorizontalHeaderLabels({
        isUk ? "Назва інгредієнта" : "Ingredient Name",
        tm.tr("inv_header_amount"),
        tm.tr("inv_header_unit")
    });
    ui->requirementsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList knownNames;
    for (const auto &item: validator.getKnownIngredientNames()) {
        knownNames << QString::fromStdString(item);
    }
    auto *completer = new QCompleter(knownNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->reqNameEdit->setCompleter(completer);

    ui->addReqButton->setStyleSheet("background-color: #2b5b84; color: white; border-radius: 6px; padding: 6px 14px;");
    ui->removeReqButton->setStyleSheet(
        "background-color: #6b2e2e; color: white; border-radius: 6px; padding: 6px 14px;");
    ui->saveButton->setStyleSheet(
        "background-color: #4a4a4a; color: white; border-radius: 6px; padding: 6px 14px; font-weight: 500;");
    ui->cancelButton->setStyleSheet(
        "background-color: #3a3a3a; color: white; border-radius: 6px; padding: 6px 14px; font-weight: 500;");

    connect(ui->reqNameEdit, &QLineEdit::textChanged, this, &AddRecipeDialog::onReqNameChanged);
    connect(ui->addReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onAddRequirementClicked);
    connect(ui->removeReqButton, &QPushButton::clicked, this, &AddRecipeDialog::onRemoveRequirementClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &AddRecipeDialog::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddRecipeDialog::~AddRecipeDialog() {
    delete ui;
}

void AddRecipeDialog::onReqNameChanged(const QString &text) {
    QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return;
    }

    auto res = validator.validate(trimmed.toStdString());
    if (res.isKnown) {
        int index = ui->reqUnitComboBox->findData(QString::fromStdString(res.expectedUnit));
        if (index != -1 && ui->reqUnitComboBox->currentIndex() != index) {
            ui->reqUnitComboBox->setCurrentIndex(index);
        }
    }
}

void AddRecipeDialog::onAddRequirementClicked() {
    auto &tm = TranslationManager::instance();
    bool isUk = (tm.getCurrentLanguage() == Language::Ukrainian);
    QString name = ui->reqNameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(
            this,
            isUk ? "Помилка валідації" : "Validation Error",
            isUk ? "Назва інгредієнта не може бути порожньою!" : "Ingredient name cannot be empty!"
        );
        return;
    }

    double amount = ui->reqAmountSpinBox->value();
    QString canonicalUnit = ui->reqUnitComboBox->currentData().toString();
    QString displayUnit = ui->reqUnitComboBox->currentText();

    auto validation = validator.validate(name.toStdString());

    if (!validation.isKnown && validation.typoSuggestion.has_value()) {
        QString suggestion = QString::fromStdString(validation.typoSuggestion.value());
        spdlog::warn("Possible typo detected in AddRecipeDialog: '{}' -> suggested '{}'",
                     name.toStdString(), suggestion.toStdString());

        auto reply = QMessageBox::question(
            this,
            isUk ? "Можлива одруківка" : "Possible Typo",
            isUk
                ? QString("Можливо, ви мали на увазі «%1» замість «%2»?").arg(suggestion, name)
                : QString("Did you mean '%1' instead of '%2'?").arg(suggestion, name),
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            name = suggestion;
            ui->reqNameEdit->setText(name);
            validation = validator.validate(name.toStdString());
            if (validation.isKnown) {
                int index = ui->reqUnitComboBox->findData(QString::fromStdString(validation.expectedUnit));
                if (index != -1) {
                    ui->reqUnitComboBox->setCurrentIndex(index);
                    canonicalUnit = ui->reqUnitComboBox->currentData().toString();
                    displayUnit = ui->reqUnitComboBox->currentText();
                }
            }
        }
    }

    if (validation.isKnown && validation.expectedUnit != canonicalUnit.toStdString()) {
        QString expectedUnitQ = tm.translateUnit(QString::fromStdString(validation.expectedUnit));

        spdlog::warn("Unit mismatch in AddRecipeDialog for '{}': selected '{}', expected '{}'",
                     name.toStdString(), canonicalUnit.toStdString(), validation.expectedUnit);

        auto reply = QMessageBox::question(
            this,
            isUk ? "Нетипова одиниця виміру" : "Unusual Measurement Unit",
            isUk
                ? QString("«%1» зазвичай вимірюється у «%2», а не «%3».\nВи впевнені, що хочете додати у «%3»?")
                      .arg(name, expectedUnitQ, displayUnit)
                : QString("'%1' is typically measured in '%2', not '%3'.\nAre you sure you want to add it in '%3'?")
                      .arg(name, expectedUnitQ, displayUnit),
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    bool merged = false;
    for (int i = 0; i < ui->requirementsTable->rowCount(); ++i) {
        QString existingName = ui->requirementsTable->item(i, 0)->text();
        QString existingCanonicalUnit = ui->requirementsTable->item(i, 2)->data(Qt::UserRole).toString();

        if (existingName.compare(name, Qt::CaseInsensitive) == 0 && existingCanonicalUnit == canonicalUnit) {
            double existingAmount = ui->requirementsTable->item(i, 1)->text().toDouble();
            ui->requirementsTable->item(i, 1)->setText(QString::number(existingAmount + amount));
            merged = true;
            break;
        }
    }

    if (!merged) {
        int row = ui->requirementsTable->rowCount();
        ui->requirementsTable->insertRow(row);
        ui->requirementsTable->setItem(row, 0, new QTableWidgetItem(name));
        ui->requirementsTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));

        auto *unitItem = new QTableWidgetItem(displayUnit);
        unitItem->setData(Qt::UserRole, canonicalUnit);
        ui->requirementsTable->setItem(row, 2, unitItem);
    }

    ui->reqNameEdit->clear();
    ui->reqAmountSpinBox->setValue(100);
}

void AddRecipeDialog::onRemoveRequirementClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    int currentRow = ui->requirementsTable->currentRow();
    if (currentRow >= 0) {
        ui->requirementsTable->removeRow(currentRow);
    } else {
        QMessageBox::warning(
            this,
            isUk ? "Помилка вибору" : "Selection Error",
            isUk ? "Будь ласка, оберіть інгредієнт для видалення!" : "Please select an ingredient to remove!"
        );
    }
}

void AddRecipeDialog::onSaveClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QString title = ui->titleLineEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(
            this,
            isUk ? "Помилка валідації" : "Validation Error",
            isUk ? "Назва рецепта не може бути порожньою!" : "Recipe title cannot be empty!"
        );
        return;
    }

    if (ui->requirementsTable->rowCount() == 0) {
        QMessageBox::warning(
            this,
            isUk ? "Помилка валідації" : "Validation Error",
            isUk ? "Будь ласка, додайте хоча б один необхідний інгредієнт!" : "Please add at least one required ingredient!"
        );
        return;
    }

    createdRecipe = core::Recipe(title.toStdString(),
                                 static_cast<size_t>(ui->prepTimeSpinBox->value()),
                                 ui->caloriesSpinBox->value(),
                                 ui->instructionsTextEdit->toPlainText().toStdString());

    for (int i = 0; i < ui->requirementsTable->rowCount(); ++i) {
        std::string reqName = ui->requirementsTable->item(i, 0)->text().toStdString();
        double reqAmount = ui->requirementsTable->item(i, 1)->text().toDouble();
        std::string reqUnit = ui->requirementsTable->item(i, 2)->data(Qt::UserRole).toString().toStdString();

        createdRecipe.addRequirement(reqName, reqAmount, reqUnit);
    }

    accept();
}

core::Recipe AddRecipeDialog::getCreatedRecipe() const {
    return createdRecipe;
}