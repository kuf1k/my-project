#include "ui/AddIngredientDialog.h"
#include "ui_AddIngredientDialog.h"
#include "ui/TranslationManager.h"
#include "core/SolidIngredient.h"
#include "core/LiquidIngredient.h"
#include "core/CountableIngredient.h"
#include <QMessageBox>
#include <QDate>
#include <QCompleter>
#include <QStringList>
#include <spdlog/spdlog.h>

AddIngredientDialog::AddIngredientDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddIngredientDialog) {
    ui->setupUi(this);

    auto &tm = TranslationManager::instance();
    bool isUk = (tm.getCurrentLanguage() == Language::Ukrainian);

    setWindowTitle(isUk ? "Додати новий інгредієнт" : "Add New Ingredient");
    ui->labelType->setText(tm.tr("lbl_type"));
    ui->labelName->setText(tm.tr("lbl_name"));
    ui->labelAmount->setText(tm.tr("lbl_amount"));
    ui->labelDate->setText(tm.tr("lbl_date"));
    ui->addButton->setText(tm.tr("btn_add"));
    ui->cancelButton->setText(tm.tr("btn_cancel"));
    ui->nameLineEdit->setPlaceholderText(
        isUk ? "напр. Cheese, Milk, Eggs" : "e.g. Cheese, Milk, Eggs"
    );

    ui->dateEdit->setDate(QDate::currentDate());

    ui->typeComboBox->addItem(isUk ? "Твердий / сипучий (г)" : "Solid (g)", "Solid");
    ui->typeComboBox->addItem(isUk ? "Рідкий (мл)" : "Liquid (ml)", "Liquid");
    ui->typeComboBox->addItem(isUk ? "Штучний (шт.)" : "Countable (pcs)", "Countable");

    QStringList knownNames;
    for (const auto &item: validator.getKnownIngredientNames()) {
        knownNames << QString::fromStdString(item);
    }
    auto *completer = new QCompleter(knownNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->nameLineEdit->setCompleter(completer);

    connect(ui->typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddIngredientDialog::onTypeChanged);

    connect(ui->nameLineEdit, &QLineEdit::textChanged,
            this, &AddIngredientDialog::onNameChanged);

    connect(ui->addButton, &QPushButton::clicked, this, &AddIngredientDialog::onAddClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    onTypeChanged(0);

    ui->addButton->setStyleSheet("background-color: #2b5b84; color: white; border-radius: 6px; padding: 6px 14px;");
    ui->cancelButton->setStyleSheet("background-color: #6b2e2e; color: white; border-radius: 6px; padding: 6px 14px;");
}

AddIngredientDialog::~AddIngredientDialog() {
    delete ui;
}

void AddIngredientDialog::onTypeChanged(int index) {
    auto &tm = TranslationManager::instance();
    QString type = ui->typeComboBox->itemData(index).toString();
    if (type == "Solid") {
        ui->unitLabel->setText(tm.translateUnit("g"));
    } else if (type == "Liquid") {
        ui->unitLabel->setText(tm.translateUnit("ml"));
    } else {
        ui->unitLabel->setText(tm.translateUnit("pcs"));
    }
}

void AddIngredientDialog::onNameChanged(const QString &text) {
    QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return;
    }

    auto res = validator.validate(trimmed.toStdString());
    if (res.isKnown) {
        int index = ui->typeComboBox->findData(QString::fromStdString(res.expectedType));
        if (index != -1 && ui->typeComboBox->currentIndex() != index) {
            ui->typeComboBox->setCurrentIndex(index);
        }
    }
}

void AddIngredientDialog::onAddClicked() {
    auto &tm = TranslationManager::instance();
    bool isUk = (tm.getCurrentLanguage() == Language::Ukrainian);
    QString name = ui->nameLineEdit->text().trimmed();
    double amount = ui->amountSpinBox->value();
    QString expDate = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString type = ui->typeComboBox->currentData().toString();

    if (name.isEmpty()) {
        QMessageBox::warning(
            this,
            isUk ? "Помилка введення" : "Input Error",
            isUk ? "Будь ласка, введіть назву інгредієнта!" : "Please enter ingredient name!"
        );
        return;
    }

    if (amount <= 0) {
        QMessageBox::warning(
            this,
            isUk ? "Помилка введення" : "Input Error",
            isUk ? "Кількість має бути більшою за нуль!" : "Amount must be greater than zero!"
        );
        return;
    }
    auto validation = validator.validate(name.toStdString());

    if (!validation.isKnown && validation.typoSuggestion.has_value()) {
        QString suggestion = QString::fromStdString(validation.typoSuggestion.value());
        spdlog::warn("Possible typo detected in AddIngredientDialog: '{}' -> suggested '{}'",
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
            ui->nameLineEdit->setText(name);
            validation = validator.validate(name.toStdString());
            if (validation.isKnown) {
                int index = ui->typeComboBox->findData(QString::fromStdString(validation.expectedType));
                if (index != -1) {
                    ui->typeComboBox->setCurrentIndex(index);
                    type = ui->typeComboBox->currentData().toString();
                }
            }
        }
    }

    if (validation.isKnown && validation.expectedType != type.toStdString()) {
        QString expectedTypeQ = QString::fromStdString(validation.expectedType);
        QString expectedUnitQ = tm.translateUnit(QString::fromStdString(validation.expectedUnit));
        QString currentUnitQ = tm.translateUnit(
            QString::fromStdString(core::IngredientValidator::getUnitForType(type.toStdString()))
        );

        spdlog::warn("Unit mismatch for '{}': selected {} ({}), expected {} ({})",
                     name.toStdString(), type.toStdString(), currentUnitQ.toStdString(),
                     validation.expectedType, validation.expectedUnit);

        auto reply = QMessageBox::question(
            this,
            isUk ? "Нетипова одиниця виміру" : "Unusual Measurement Type",
            isUk
                ? QString("«%1» зазвичай вимірюється як %2 (%3), а не %4 (%5).\nВи впевнені, що хочете додати як %4?")
                      .arg(name, expectedTypeQ, expectedUnitQ, type, currentUnitQ)
                : QString("'%1' is typically measured as %2 (%3), not %4 (%5).\nAre you sure you want to add it as %4?")
                      .arg(name, expectedTypeQ, expectedUnitQ, type, currentUnitQ),
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No) {
            return;
        }
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