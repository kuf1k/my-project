#include "ui/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/AddIngredientDialog.h"
#include "ui/AddRecipeDialog.h"
#include "ui/TranslationManager.h"
#include "core/StorageManager.h"
#include "core/DateUtils.h"
#include "core/FileExporter.h"
#include "core/DataFilter.h"
#include "core/RecipeMatcher.h"
#include <QHeaderView>
#include <QColor>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QComboBox>
#include <QHBoxLayout>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDate>
#include <QTime>
#include <QMap>
#include <QSet>
#include <QSignalBlocker>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto *topHeaderLayout = new QHBoxLayout();
    languageButton = new QPushButton("🌐 UA", this);
    languageButton->setObjectName("languageButton");
    languageButton->setCursor(Qt::PointingHandCursor);

    ui->mainLayout->removeWidget(ui->headerTitleLabel);
    topHeaderLayout->addStretch();
    topHeaderLayout->addWidget(ui->headerTitleLabel);
    topHeaderLayout->addStretch();
    topHeaderLayout->addWidget(languageButton);
    ui->mainLayout->insertLayout(0, topHeaderLayout);

    connect(languageButton, &QPushButton::clicked, this, &MainWindow::onToggleLanguageClicked);

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

    connect(ui->refreshStatisticsButton, &QPushButton::clicked, this, &MainWindow::onRefreshStatisticsClicked);

    setupInventoryTable();
    setupShoppingTable();
    setupMealPlanTable();
    loadData();
    updateInventoryTable();
    updateRecipesList();
    updateShoppingRecipeComboBox();
    updateMealPlanTable();
    onApplySmartFilterClicked();
    updateStatisticsTab();
    retranslateUi();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onToggleLanguageClicked() {
    auto &tm = TranslationManager::instance();
    if (tm.getCurrentLanguage() == Language::Ukrainian) {
        tm.setLanguage(Language::English);
        languageButton->setText("🌐 EN");
    } else {
        tm.setLanguage(Language::Ukrainian);
        languageButton->setText("🌐 UA");
    }
    retranslateUi();
}

void MainWindow::retranslateUi() {
    auto &tm = TranslationManager::instance();
    bool isUk = (tm.getCurrentLanguage() == Language::Ukrainian);


    setWindowTitle(isUk ? "NutriMesh - Розумний кухонний помічник" : "NutriMesh - Smart Kitchen Assistant");
    ui->headerTitleLabel->setText(
        QString("<h2 style='margin:0; padding:2px 0;'>%1</h2>").arg(tm.tr("app_title"))
    );
    ui->tabWidget->setTabText(0, tm.tr("tab_pantry"));
    ui->tabWidget->setTabText(1, tm.tr("tab_matcher"));
    ui->tabWidget->setTabText(2, tm.tr("tab_recipes"));
    ui->tabWidget->setTabText(3, tm.tr("tab_shopping"));
    ui->tabWidget->setTabText(4, tm.tr("tab_meal"));
    ui->tabWidget->setTabText(5, tm.tr("tab_filter"));
    ui->tabWidget->setTabText(6, tm.tr("tab_stats"));


    ui->addIngredientButton->setText(tm.tr("btn_add_ingredient"));
    ui->deleteIngredientButton->setText(tm.tr("btn_delete_ingredient"));
    ui->removeExpiredButton->setText(tm.tr("btn_remove_expired"));
    ui->exportInventoryButton->setText(tm.tr("btn_export_inventory"));
    ui->refreshButton->setText(tm.tr("btn_refresh_inventory"));

    ui->inventoryTable->setHorizontalHeaderLabels({
        tm.tr("inv_header_name"),
        tm.tr("inv_header_amount"),
        tm.tr("inv_header_unit"),
        tm.tr("inv_header_exp")
    });


    ui->findRecipesButton->setText(tm.tr("btn_find_recipes"));
    ui->matcherSortLabel->setText(tm.tr("lbl_sort_by"));
    ui->cookRecipeButton->setText(tm.tr("btn_cook_recipe"));
    ui->cookableRecipeDetailsTextEdit->setPlaceholderText(
        isUk
            ? "Натисніть «Знайти рецепти» та оберіть страву зі списку для перегляду деталей..."
            : "Click 'Find Recipes' and select a recipe to view details..."
    );

    ui->recipesSortLabel->setText(tm.tr("lbl_sort_by"));
    ui->addRecipeButton->setText(tm.tr("btn_add_recipe"));
    ui->recipeDetailsTextEdit->setPlaceholderText(
        isUk
            ? "Оберіть рецепт зі списку ліворуч для перегляду деталей..."
            : "Select a recipe from the list to view details..."
    );


    int recSortIdx = ui->recipesSortComboBox->currentIndex();
    int matchSortIdx = ui->matcherSortComboBox->currentIndex();
    {
        QSignalBlocker blocker1(ui->recipesSortComboBox);
        QSignalBlocker blocker2(ui->matcherSortComboBox);
        setupSortComboBoxes();
        if (recSortIdx >= 0) ui->recipesSortComboBox->setCurrentIndex(recSortIdx);
        if (matchSortIdx >= 0) ui->matcherSortComboBox->setCurrentIndex(matchSortIdx);
    }

    ui->shoppingRecipeLabel->setText(tm.tr("lbl_select_recipe"));
    ui->generateShoppingListButton->setText(tm.tr("btn_gen_shopping"));
    ui->exportShoppingListButton->setText(tm.tr("btn_export_shopping"));
    ui->shoppingTableWidget->setHorizontalHeaderLabels(
        isUk
            ? QStringList{"Назва інгредієнта", "Потрібно докупити", "Одиниця"}
            : QStringList{"Ingredient Name", "Amount Needed to Buy", "Unit"}
    );


    ui->mealDayLabel->setText(tm.tr("lbl_day"));
    ui->mealTimeLabel->setText(tm.tr("lbl_time"));
    ui->mealRecipeLabel->setText(tm.tr("lbl_recipe"));
    ui->addMealPlanButton->setText(tm.tr("btn_add_schedule"));
    ui->removeMealPlanButton->setText(tm.tr("btn_delete_ingredient"));
    ui->clearMealPlanButton->setText(tm.tr("btn_clear_schedule"));
    ui->generateWeeklyShoppingButton->setText(tm.tr("btn_weekly_shopping"));
    ui->mealPlanTableWidget->setHorizontalHeaderLabels(
        isUk
            ? QStringList{"День", "Час", "Рецепт", "Час готування (хв)", "Калорії (ккал)"}
            : QStringList{"Day", "Time", "Recipe", "Prep Time (mins)", "Calories (kcal)"}
    );


    int mealDayIdx = ui->mealDayComboBox->currentIndex();
    int quickDayIdx = ui->quickScheduleDayComboBox->currentIndex();
    setupMealPlannerControls();
    setupSmartFilterControls();
    if (mealDayIdx >= 0) ui->mealDayComboBox->setCurrentIndex(mealDayIdx);
    if (quickDayIdx >= 0) ui->quickScheduleDayComboBox->setCurrentIndex(quickDayIdx);


    ui->filterMaxCalLabel->setText(tm.tr("lbl_max_calories"));
    ui->filterMaxTimeLabel->setText(tm.tr("lbl_max_time"));
    ui->filterIngredientLabel->setText(isUk ? "Інгредієнт:" : "Ingredient:");
    ui->filterIngredientLineEdit->setPlaceholderText(isUk ? "напр. Milk, Eggs..." : "e.g. Milk, Eggs...");
    ui->filterMaxCaloriesSpinBox->setSuffix(isUk ? " ккал" : " kcal");
    ui->filterMaxTimeSpinBox->setSuffix(isUk ? " хв" : " mins");
    ui->applySmartFilterButton->setText(tm.tr("btn_apply_filter"));
    ui->resetSmartFilterButton->setText(tm.tr("btn_reset"));
    ui->quickAddToMealPlanButton->setText(isUk ? "📅 Додати до планувальника" : "📅 Add to Meal Planner");
    ui->filteredRecipeDetailsTextEdit->setPlaceholderText(
        isUk
            ? "Задайте критерії фільтрації вище та оберіть рецепт зі списку..."
            : "Set filter criteria above and select a recipe to view details..."
    );


    ui->refreshStatisticsButton->setText(tm.tr("btn_refresh_stats"));


    updateInventoryTable();
    updateMealPlanTable();
    updateStatisticsTab();
    onRecipeSelectionChanged();
    onCookableRecipeSelectionChanged();
    onFilteredRecipeSelectionChanged();
}

QString MainWindow::formatDayWithDate(const QString &dayName) const {
    static const QMap<QString, int> dayNumbers = {
        {"Monday", 1}, {"Tuesday", 2}, {"Wednesday", 3}, {"Thursday", 4},
        {"Friday", 5}, {"Saturday", 6}, {"Sunday", 7}
    };

    static const QMap<QString, QString> ukDayNames = {
        {"Monday", "Понеділок"}, {"Tuesday", "Вівторок"}, {"Wednesday", "Середа"},
        {"Thursday", "Четвер"}, {"Friday", "П'ятниця"}, {"Saturday", "Субота"},
        {"Sunday", "Неділя"}
    };

    int targetDay = dayNumbers.value(dayName, 0);
    if (targetDay == 0) {
        return dayName;
    }

    QDate today = QDate::currentDate();
    int diff = (targetDay - today.dayOfWeek() + 7) % 7;
    QDate targetDate = today.addDays(diff);

    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QString displayDay = isUk ? ukDayNames.value(dayName, dayName) : dayName;

    return QString("%1 (%2)").arg(displayDay, targetDate.toString("dd.MM"));
}

void MainWindow::setupSortComboBoxes() {

    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QStringList options = isUk
        ? QStringList{
            "За назвою (A-Z)",
            "За часом готування (найшвидші)",
            "За калоріями (від менших)",
            "За калоріями (від більших)"
        }
        : QStringList{
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
    for (const QString &day : days) {
        ui->mealDayComboBox->addItem(formatDayWithDate(day), day);
    }
    ui->mealTimeEdit->setTime(QTime(10, 0));
}

void MainWindow::setupSmartFilterControls() {
    QStringList days = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    ui->quickScheduleDayComboBox->clear();
    for (const QString &day : days) {
        ui->quickScheduleDayComboBox->addItem(formatDayWithDate(day), day);
    }
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

        QString formattedDay = formatDayWithDate(QString::fromStdString(entries[i].dayOfWeek));
        ui->mealPlanTableWidget->setItem(i, 0, new QTableWidgetItem(formattedDay));
        ui->mealPlanTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(entries[i].time)));
        ui->mealPlanTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(entries[i].recipeTitle)));
        ui->mealPlanTableWidget->setItem(i, 3, new QTableWidgetItem(prepStr));
        ui->mealPlanTableWidget->setItem(i, 4, new QTableWidgetItem(calStr));
    }

    double totalCal = mealPlanner.getTotalCalories(recipeRepo);
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    ui->mealTotalCaloriesLabel->setText(
        isUk
            ? QString("<b>Загальна запланована калорійність:</b> %1 ккал").arg(totalCal)
            : QString("<b>Total Scheduled Calories:</b> %1 kcal").arg(totalCal)
    );
}

void MainWindow::updateStatisticsTab() {
    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    const auto &items = inventory.getItems();
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);

    size_t solidCount = 0, liquidCount = 0, countableCount = 0;
    double totalSolidWeight = 0.0, totalLiquidVolume = 0.0, totalCountableQty = 0.0;
    size_t freshCount = 0, expiringSoonCount = 0, expiredCount = 0;

    struct ExpiringItemInfo {
        QString name;
        double amount;
        QString unit;
        QString expDate;
        int daysLeft;
    };
    QVector<ExpiringItemInfo> expiringItems;
    QSet<QString> expiringNamesLower;

    for (const auto &item : items) {
        if (item->getUnit() == "g") {
            solidCount++;
            totalSolidWeight += item->getAmount();
        } else if (item->getUnit() == "ml") {
            liquidCount++;
            totalLiquidVolume += item->getAmount();
        } else if (item->getUnit() == "pcs") {
            countableCount++;
            totalCountableQty += item->getAmount();
        }

        int daysLeft = core::DateUtils::getDaysUntil(item->getExpirationDate(), currentDate);
        if (daysLeft < 0) {
            expiredCount++;
        } else if (daysLeft <= 3) {
            expiringSoonCount++;
            QString qName = QString::fromStdString(item->getName());
            expiringNamesLower.insert(qName.toLower());
            expiringItems.push_back({
                qName,
                item->getAmount(),
                QString::fromStdString(item->getUnit()),
                QString::fromStdString(item->getExpirationDate()),
                daysLeft
            });
        } else {
            freshCount++;
        }
    }

    const auto &recipes = recipeRepo.getRecipes();
    size_t cookableNowCount = recipeRepo.getCookableRecipes(inventory, currentDate).size();
    double avgCalories = 0.0;
    if (!recipes.empty()) {
        double sumCal = 0.0;
        for (const auto &r : recipes) {
            sumCal += r.getCalories();
        }
        avgCalories = sumCal / static_cast<double>(recipes.size());
    }

    QString leftHtml = isUk
        ? "<h2>📊 Аналітика кухні та комори</h2><h3>🧊 Розподіл запасів</h3><ul>"
        : "<h2>📊 Kitchen &amp; Pantry Analytics</h2><h3>🧊 Pantry Stock Breakdown</h3><ul>";

    if (isUk) {
        leftHtml += QString("<li><b>Усього інгредієнтів на зберіганні:</b> %1</li>").arg(items.size());
        leftHtml += QString("<li><b>Сипучі / тверді:</b> %1 поз. (%2 g)</li>").arg(solidCount).arg(totalSolidWeight);
        leftHtml += QString("<li><b>Рідкі:</b> %1 поз. (%2 ml)</li>").arg(liquidCount).arg(totalLiquidVolume);
        leftHtml += QString("<li><b>Штучні:</b> %1 поз. (%2 pcs)</li>").arg(countableCount).arg(totalCountableQty);
        leftHtml += "</ul><h3>🛡️ Стан свіжості</h3><ul>";
        leftHtml += QString("<li><span style='color:#66bb6a;'><b>Свіжі (&gt; 3 дн.):</b></span> %1</li>").arg(freshCount);
        leftHtml += QString("<li><span style='color:#ffca28;'><b>Скоро зіпсуються (0–3 дн.):</b></span> %1</li>").arg(expiringSoonCount);
        leftHtml += QString("<li><span style='color:#ef5350;'><b>Протерміновані (&lt; 0 дн.):</b></span> %1</li>").arg(expiredCount);
        leftHtml += "</ul><h3>📖 Рецепти та план на тиждень</h3><ul>";
        leftHtml += QString("<li><b>Рецептів у книзі:</b> %1 (Доступні зараз: <b>%2</b>)</li>")
                .arg(recipes.size()).arg(cookableNowCount);
        leftHtml += QString("<li><b>Середня калорійність страви:</b> %1 ккал</li>")
                .arg(QString::number(avgCalories, 'f', 0));
        leftHtml += QString("<li><b>Заплановано прийомів їжі:</b> %1 (%2 ккал разом)</li>")
                .arg(mealPlanner.getEntriesCount())
                .arg(mealPlanner.getTotalCalories(recipeRepo));
        leftHtml += "</ul>";
    } else {
        leftHtml += QString("<li><b>Total Stored Ingredients:</b> %1</li>").arg(items.size());
        leftHtml += QString("<li><b>Solid Ingredients:</b> %1 item(s) (%2 g)</li>").arg(solidCount).arg(totalSolidWeight);
        leftHtml += QString("<li><b>Liquid Ingredients:</b> %1 item(s) (%2 ml)</li>").arg(liquidCount).arg(totalLiquidVolume);
        leftHtml += QString("<li><b>Countable Ingredients:</b> %1 item(s) (%2 pcs)</li>").arg(countableCount).arg(totalCountableQty);
        leftHtml += "</ul><h3>🛡️ Freshness Status</h3><ul>";
        leftHtml += QString("<li><span style='color:#66bb6a;'><b>Fresh (&gt; 3 days):</b></span> %1</li>").arg(freshCount);
        leftHtml += QString("<li><span style='color:#ffca28;'><b>Expiring Soon (0–3 days):</b></span> %1</li>").arg(expiringSoonCount);
        leftHtml += QString("<li><span style='color:#ef5350;'><b>Expired (&lt; 0 days):</b></span> %1</li>").arg(expiredCount);
        leftHtml += "</ul><h3>📖 Recipes &amp; Weekly Plan</h3><ul>";
        leftHtml += QString("<li><b>Recipes in Book:</b> %1 (Cookable right now: <b>%2</b>)</li>")
                .arg(recipes.size()).arg(cookableNowCount);
        leftHtml += QString("<li><b>Average Recipe Calories:</b> %1 kcal</li>")
                .arg(QString::number(avgCalories, 'f', 0));
        leftHtml += QString("<li><b>Scheduled Meals This Week:</b> %1 (%2 kcal total)</li>")
                .arg(mealPlanner.getEntriesCount())
                .arg(mealPlanner.getTotalCalories(recipeRepo));
        leftHtml += "</ul>";
    }

    ui->pantryStatsTextEdit->setHtml(leftHtml);

    QString rightHtml = isUk ? "<h2>⚠️ Радник порятунку продуктів</h2>" : "<h2>⚠️ Rescue Advisor</h2>";
    if (expiringItems.isEmpty()) {
        rightHtml += isUk
            ? "<p style='color:#66bb6a;'><b>Усе чудово!</b> Немає продуктів, термін яких спливає протягом найближчих 3 днів.</p>"
            : "<p style='color:#66bb6a;'><b>All good!</b> No ingredients are expiring within the next 3 days.</p>";
    } else {
        rightHtml += isUk
            ? "<p>Ці інгредієнти скоро зіпсуються (ще придатні для приготування!):</p><ul>"
            : "<p>These ingredients will expire soon (still safe to cook with!):</p><ul>";

        for (const auto &exp : expiringItems) {
            QString badge;
            if (isUk) {
                badge = (exp.daysLeft == 0)
                    ? "<span style='color:#ff7043;'><b>Спливає СЬОГОДНІ!</b></span>"
                    : QString("<span style='color:#ffca28;'>за %1 дн.</span>").arg(exp.daysLeft);
                rightHtml += QString("<li><b>%1</b> (%2 %3) — %4 (до %5)</li>")
                        .arg(exp.name).arg(exp.amount).arg(exp.unit).arg(badge).arg(exp.expDate);
            } else {
                badge = (exp.daysLeft == 0)
                    ? "<span style='color:#ff7043;'><b>Expires TODAY!</b></span>"
                    : QString("<span style='color:#ffca28;'>in %1 day(s)</span>").arg(exp.daysLeft);
                rightHtml += QString("<li><b>%1</b> (%2 %3) — %4 (until %5)</li>")
                        .arg(exp.name).arg(exp.amount).arg(exp.unit).arg(badge).arg(exp.expDate);
            }
        }
        rightHtml += "</ul>";

        auto rescueRecipes = core::DataFilter<core::Recipe>::filterBy(
            recipes,
            [&expiringNamesLower](const core::Recipe &recipe) {
                return core::DataFilter<core::RecipeRequirement>::contains(
                    recipe.getRequirements(),
                    [&expiringNamesLower](const core::RecipeRequirement &req) {
                        return expiringNamesLower.contains(QString::fromStdString(req.name).toLower());
                    }
                );
            }
        );

        rightHtml += isUk
            ? "<h3>🍳 Рекомендовані рецепти для порятунку продуктів:</h3>"
            : "<h3>🍳 Recommended Recipes to Save Food:</h3>";

        if (rescueRecipes.empty()) {
            rightHtml += isUk
                ? "<p>У книзі рецептів поки немає страв із цими інгредієнтами.</p>"
                : "<p>No recipes in your book use these specific expiring ingredients yet.</p>";
        } else {
            rightHtml += "<ul>";
            for (const auto &r : rescueRecipes) {
                bool readyNow = core::RecipeMatcher::canPrepare(r, inventory, currentDate);
                QString statusTag;
                if (isUk) {
                    statusTag = readyNow
                        ? "<span style='color:#66bb6a;'>[Можна готувати!]</span>"
                        : "<span style='color:#b0b0b0;'>[Бракує деяких інгредієнтів]</span>";
                    rightHtml += QString("<li><b>%1</b> (%2 хв, %3 ккал) %4</li>")
                            .arg(QString::fromStdString(r.getTitle()))
                            .arg(r.getPrepTime())
                            .arg(r.getCalories())
                            .arg(statusTag);
                } else {
                    statusTag = readyNow
                        ? "<span style='color:#66bb6a;'>[Ready to Cook!]</span>"
                        : "<span style='color:#b0b0b0;'>[Missing some items]</span>";
                    rightHtml += QString("<li><b>%1</b> (%2 mins, %3 kcal) %4</li>")
                            .arg(QString::fromStdString(r.getTitle()))
                            .arg(r.getPrepTime())
                            .arg(r.getCalories())
                            .arg(statusTag);
                }
            }
            rightHtml += "</ul>";
        }
    }

    ui->expiringRescueTextEdit->setHtml(rightHtml);
}

void MainWindow::displayRecipeDetails(const core::Recipe &recipe, QTextEdit *targetTextEdit) {
    if (!targetTextEdit) return;
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);

    QString html = QString("<h2>%1</h2>").arg(QString::fromStdString(recipe.getTitle()));
    html += isUk
        ? QString("<p style='line-height: 1.4;'><b>⏱️ Час приготування:</b> %1 хв<br><b>🔥 Калорійність:</b> %2 ккал</p>")
              .arg(recipe.getPrepTime()).arg(recipe.getCalories())
        : QString("<p style='line-height: 1.4;'><b>⏱️ Prep Time:</b> %1 mins<br><b>🔥 Calories:</b> %2 kcal</p>")
              .arg(recipe.getPrepTime()).arg(recipe.getCalories());

    html += isUk ? "<h3>🛒 Необхідні інгредієнти:</h3><ul>" : "<h3>🛒 Required Ingredients:</h3><ul>";
    for (const auto &req: recipe.getRequirements()) {
        html += QString("<li><b>%1</b>: %2 %3</li>")
                .arg(QString::fromStdString(req.name))
                .arg(req.requiredAmount)
                .arg(QString::fromStdString(req.unit));
    }
    html += "</ul>";

    html += isUk ? "<h3>📝 Інструкції:</h3>" : "<h3>📝 Instructions:</h3>";
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
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    auto cookableRecipes = recipeRepo.getCookableRecipes(inventory, currentDate);

    if (cookableRecipes.empty()) {
        spdlog::info("User checked cookable recipes: None available with current pantry items.");
        QMessageBox::information(
            this,
            isUk ? "Немає підходящих рецептів" : "No Matching Recipes",
            isUk
                ? "На жаль, у коморі зараз недостатньо свіжих інгредієнтів для жодного рецепта!"
                : "Sorry, you don't have enough ingredients in your pantry for any recipe right now!"
        );
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
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QListWidgetItem *selectedItem = ui->cookableRecipesListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(
            this,
            isUk ? "Оберіть рецепт" : "Select Recipe",
            isUk ? "Будь ласка, спочатку оберіть доступний рецепт!" : "Please select a cookable recipe first!"
        );
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

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    if (inventory.consumeRecipe(*targetRecipe, currentDate)) {
        spdlog::info("User cooked recipe: '{}'. Pantry ingredients deducted.", title);
        core::StorageManager::saveInventory(inventory, "inventory_save.json");
        updateInventoryTable();
        updateStatisticsTab();

        ui->cookableRecipesListWidget->clear();
        ui->cookableRecipeDetailsTextEdit->clear();

        auto cookableRecipes = recipeRepo.getCookableRecipes(inventory, currentDate);
        if (!cookableRecipes.empty()) {
            int currentIndex = ui->matcherSortComboBox->currentIndex();
            if (currentIndex < 0) currentIndex = 0;
            auto sortOption = static_cast<core::RecipeSortOption>(currentIndex);
            cookableRecipes = core::RecipeRepository::sortRecipes(cookableRecipes, sortOption);

            for (const auto &recipe: cookableRecipes) {
                ui->cookableRecipesListWidget->addItem(QString::fromStdString(recipe.getTitle()));
            }
        }

        QMessageBox::information(
            this,
            isUk ? "Смачного! 🍽️" : "Bon Appétit! 🍽️",
            isUk
                ? QString("Ви успішно приготували «%1»! Необхідні інгредієнти списано з комори.").arg(QString::fromStdString(title))
                : QString("You successfully cooked '%1'! Required ingredients have been deducted from your pantry.").arg(QString::fromStdString(title))
        );
    } else {
        spdlog::warn("User failed to cook recipe: '{}'. Insufficient ingredients.", title);
        QMessageBox::critical(
            this,
            isUk ? "Помилка" : "Error",
            isUk ? "У коморі недостатньо свіжих інгредієнтів для приготування цієї страви!" : "Not enough ingredients in your pantry to cook this recipe!"
        );
    }
}

void MainWindow::onAddRecipeClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    AddRecipeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        core::Recipe newRecipe = dialog.getCreatedRecipe();

        if (recipeRepo.hasRecipe(newRecipe.getTitle())) {
            spdlog::warn("Attempted to add duplicate recipe: '{}'", newRecipe.getTitle());
            QMessageBox::warning(
                this,
                isUk ? "Дублікат рецепта" : "Duplicate Recipe",
                isUk
                    ? QString("Рецепт «%1» уже існує!").arg(QString::fromStdString(newRecipe.getTitle()))
                    : QString("Recipe '%1' already exists!").arg(QString::fromStdString(newRecipe.getTitle()))
            );
            return;
        }

        recipeRepo.addRecipe(newRecipe);
        spdlog::info("User added new recipe: '{}' ({} mins, {} kcal)", newRecipe.getTitle(), newRecipe.getPrepTime(),
                     newRecipe.getCalories());
        core::StorageManager::saveRecipes(recipeRepo, "recipes_save.json");

        updateRecipesList();
        updateShoppingRecipeComboBox();
        onApplySmartFilterClicked();
        updateStatisticsTab();

        QMessageBox::information(
            this,
            isUk ? "Успіх" : "Success",
            isUk ? "Власний рецепт успішно додано!" : "Custom recipe added successfully!"
        );
    }
}

void MainWindow::onDeleteIngredientClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    int selectedRow = ui->inventoryTable->currentRow();
    const auto &items = inventory.getItems();

    if (selectedRow < 0 || selectedRow >= static_cast<int>(items.size())) {
        QMessageBox::warning(
            this,
            isUk ? "Нічого не обрано" : "No Selection",
            isUk ? "Будь ласка, спочатку оберіть рядок інгредієнта в таблиці!" : "Please select an ingredient row in the table first!"
        );
        return;
    }

    QString itemName = QString::fromStdString(items[selectedRow]->getName());
    double itemAmount = items[selectedRow]->getAmount();
    QString itemUnit = QString::fromStdString(items[selectedRow]->getUnit());

    auto reply = QMessageBox::question(
        this,
        isUk ? "Підтвердження видалення" : "Confirm Deletion",
        isUk
            ? QString("Ви впевнені, що хочете видалити «%1» (%2 %3)?").arg(itemName).arg(itemAmount).arg(itemUnit)
            : QString("Are you sure you want to delete '%1' (%2 %3)?").arg(itemName).arg(itemAmount).arg(itemUnit),
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
        updateStatisticsTab();
    }
}

void MainWindow::onRemoveExpiredClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    size_t removedCount = inventory.removeExpired(currentDate);

    if (removedCount == 0) {
        spdlog::info("User triggered expired items removal: Pantry is clean.");
        QMessageBox::information(
            this,
            isUk ? "Комора чиста" : "Clean Pantry",
            isUk ? "Протермінованих інгредієнтів у коморі не знайдено!" : "No expired ingredients found in your pantry!"
        );
        return;
    }

    spdlog::warn("User removed {} expired ingredient(s) from pantry.", removedCount);
    core::StorageManager::saveInventory(inventory, "inventory_save.json");
    updateInventoryTable();
    updateStatisticsTab();

    QMessageBox::information(
        this,
        isUk ? "Протерміноване видалено" : "Expired Removed",
        isUk
            ? QString("Успішно видалено %1 протермінованих інгредієнт(ів) з комори.").arg(removedCount)
            : QString("Successfully removed %1 expired ingredient(s) from your pantry.").arg(removedCount)
    );
}

void MainWindow::onExportInventoryClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    if (inventory.getItemsCount() == 0) {
        spdlog::warn("Attempted to export empty inventory.");
        QMessageBox::warning(
            this,
            isUk ? "Попередження експорту" : "Export Warning",
            isUk ? "Ваша комора порожня! Нічого експортувати." : "Your pantry is empty! Nothing to export."
        );
        return;
    }

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                          + "/inventory_report.txt";

    QString fileName = QFileDialog::getSaveFileName(
        this,
        isUk ? "Експортувати звіт інвентарю" : "Export Inventory Report",
        defaultPath,
        isUk ? "Текстові файли (*.txt);;Усі файли (*)" : "Text Files (*.txt);;All Files (*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    core::FileExporter exporter("Pantry Inventory Report", fileName.toStdString());

    if (exporter.exportData(inventory, currentDate)) {
        spdlog::info("Exported inventory report to: {}", fileName.toStdString());
        QMessageBox::information(
            this,
            isUk ? "Успіх" : "Success",
            (isUk ? "Звіт інвентарю успішно збережено у " : "Inventory report successfully exported to ") + fileName
        );
    } else {
        spdlog::error("Failed to export inventory report to: {}", fileName.toStdString());
        QMessageBox::critical(
            this,
            isUk ? "Помилка" : "Error",
            isUk ? "Не вдалося зберегти звіт інвентарю у файл." : "Failed to export inventory report to file."
        );
    }
}

void MainWindow::onGenerateShoppingListClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QString selectedTitle = ui->shoppingRecipeComboBox->currentText();
    if (selectedTitle.isEmpty()) {
        QMessageBox::warning(
            this,
            isUk ? "Попередження" : "Warning",
            isUk ? "Будь ласка, спочатку оберіть рецепт!" : "Please select a recipe first!"
        );
        return;
    }

    std::string title = selectedTitle.toStdString();
    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    bool found = false;

    for (const auto &recipe: recipeRepo.getRecipes()) {
        if (recipe.getTitle() == title) {
            shoppingList.generateForRecipe(recipe, inventory, currentDate);
            found = true;
            break;
        }
    }

    if (!found) return;

    ui->shoppingTableWidget->setRowCount(0);
    const auto &items = shoppingList.getItems();

    if (items.empty()) {
        spdlog::info("Generated shopping list for '{}': All ingredients already available.", title);
        QMessageBox::information(
            this,
            isUk ? "Список покупок" : "Shopping List",
            isUk
                ? "У вас уже є всі необхідні свіжі інгредієнти в коморі для страви «" + selectedTitle + "»!"
                : "You already have all the necessary ingredients in your pantry for " + selectedTitle + "!"
        );
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
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    if (shoppingList.isEmpty()) {
        spdlog::warn("Attempted to export an empty shopping list.");
        QMessageBox::warning(
            this,
            isUk ? "Попередження експорту" : "Export Warning",
            isUk ? "Список покупок порожній! Спочатку згенеруйте його." : "The shopping list is empty! Generate a shopping list first."
        );
        return;
    }

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                          + "/shopping_list.txt";

    QString fileName = QFileDialog::getSaveFileName(
        this,
        isUk ? "Експортувати список покупок" : "Export Shopping List",
        defaultPath,
        isUk ? "Текстові файли (*.txt);;Усі файли (*)" : "Text Files (*.txt);;All Files (*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    if (core::FileExporter::exportShoppingListToTxt(shoppingList, fileName.toStdString())) {
        spdlog::info("Exported shopping list to: {}", fileName.toStdString());
        QMessageBox::information(
            this,
            isUk ? "Успіх" : "Success",
            (isUk ? "Список покупок успішно збережено у " : "Shopping list successfully exported to ") + fileName
        );
    } else {
        spdlog::error("Failed to export shopping list to: {}", fileName.toStdString());
        QMessageBox::critical(
            this,
            isUk ? "Помилка" : "Error",
            isUk ? "Не вдалося зберегти список покупок у файл." : "Failed to export shopping list to file."
        );
    }
}

void MainWindow::onAddMealPlanClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QString day = ui->mealDayComboBox->currentData().toString();
    QString time = ui->mealTimeEdit->time().toString("HH:mm");
    QString recipeTitle = ui->mealRecipeComboBox->currentText();

    if (recipeTitle.isEmpty()) {
        QMessageBox::warning(
            this,
            isUk ? "Попередження" : "Warning",
            isUk ? "Немає доступних рецептів! Спочатку додайте рецепт." : "No recipes available! Please add a recipe first."
        );
        return;
    }

    mealPlanner.addMeal(day.toStdString(), time.toStdString(), recipeTitle.toStdString());
    core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
    updateMealPlanTable();
    updateStatisticsTab();

    spdlog::info("Scheduled meal '{}' on {} at {}", recipeTitle.toStdString(), day.toStdString(), time.toStdString());
}

void MainWindow::onRemoveMealPlanClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    int selectedRow = ui->mealPlanTableWidget->currentRow();
    if (selectedRow < 0 || selectedRow >= static_cast<int>(mealPlanner.getEntriesCount())) {
        QMessageBox::warning(
            this,
            isUk ? "Нічого не обрано" : "No Selection",
            isUk ? "Будь ласка, спочатку оберіть рядок запланованої страви!" : "Please select a scheduled meal row first!"
        );
        return;
    }

    if (mealPlanner.removeMealAt(static_cast<size_t>(selectedRow))) {
        core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
        updateMealPlanTable();
        updateStatisticsTab();
        spdlog::info("Removed scheduled meal at row {}", selectedRow);
    }
}

void MainWindow::onClearMealPlanClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    if (mealPlanner.getEntriesCount() == 0) {
        return;
    }

    auto reply = QMessageBox::question(
        this,
        isUk ? "Очищення розкладу" : "Clear Schedule",
        isUk ? "Ви впевнені, що хочете очистити весь тижневий план харчування?" : "Are you sure you want to clear the entire weekly meal schedule?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        mealPlanner.clear();
        core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
        updateMealPlanTable();
        updateStatisticsTab();
        spdlog::info("User cleared the weekly meal schedule.");
    }
}

void MainWindow::onGenerateWeeklyShoppingClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    auto plannedRecipes = mealPlanner.getPlannedRecipes(recipeRepo);
    if (plannedRecipes.empty()) {
        QMessageBox::warning(
            this,
            isUk ? "Порожній розклад" : "Empty Schedule",
            isUk ? "Ваш розклад харчування порожній! Спочатку додайте страви." : "Your meal schedule is empty! Add some meals first."
        );
        return;
    }

    std::string currentDate = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
    shoppingList.generateForRecipes(plannedRecipes, inventory, currentDate);

    ui->shoppingTableWidget->setRowCount(0);
    const auto &items = shoppingList.getItems();

    ui->tabWidget->setCurrentWidget(ui->tabShopping);

    if (items.empty()) {
        QMessageBox::information(
            this,
            isUk ? "Усе готово!" : "All Set!",
            isUk ? "У вас уже є всі необхідні свіжі інгредієнти для всього тижневого плану!" : "You already have all the ingredients needed for your entire weekly meal plan!"
        );
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
    QMessageBox::information(
        this,
        isUk ? "Тижневий список покупок готовий" : "Weekly Shopping List Ready",
        isUk
            ? QString("Згенеровано список покупок для %1 запланованих страв!").arg(plannedRecipes.size())
            : QString("Generated shopping list for %1 scheduled meal(s)!").arg(plannedRecipes.size())
    );
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
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    QListWidgetItem *selectedItem = ui->filteredRecipesListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(
            this,
            isUk ? "Нічого не обрано" : "No Selection",
            isUk ? "Будь ласка, спочатку оберіть рецепт із відфільтрованого списку!" : "Please select a recipe from the filtered list first!"
        );
        return;
    }

    QString day = ui->quickScheduleDayComboBox->currentData().toString();
    QString displayDay = ui->quickScheduleDayComboBox->currentText();
    QString time = ui->quickScheduleTimeEdit->time().toString("HH:mm");
    QString recipeTitle = selectedItem->text();

    mealPlanner.addMeal(day.toStdString(), time.toStdString(), recipeTitle.toStdString());
    core::StorageManager::saveMealPlan(mealPlanner, "meal_plan_save.json");
    updateMealPlanTable();
    updateStatisticsTab();

    spdlog::info("Quick-scheduled filtered recipe '{}' on {} at {}",
                 recipeTitle.toStdString(), day.toStdString(), time.toStdString());

    QMessageBox::information(
        this,
        isUk ? "Додано до розкладу" : "Added to Schedule",
        isUk
            ? QString("«%1» заплановано на %2 о %3!").arg(recipeTitle, displayDay, time)
            : QString("'%1' scheduled for %2 at %3!").arg(recipeTitle, displayDay, time)
    );
}

void MainWindow::onRefreshStatisticsClicked() {
    updateStatisticsTab();
    spdlog::info("User manually refreshed kitchen statistics & analytics.");
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
    updateStatisticsTab();
}

void MainWindow::onAddIngredientClicked() {
    bool isUk = (TranslationManager::instance().getCurrentLanguage() == Language::Ukrainian);
    AddIngredientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto newIngredient = dialog.getCreatedIngredient();
        if (newIngredient) {
            spdlog::info("User added ingredient: '{}' ({:.1f} {})", newIngredient->getName(),
                         newIngredient->getAmount(), newIngredient->getUnit());
            inventory.addIngredient(std::move(newIngredient));
            core::StorageManager::saveInventory(inventory, "inventory_save.json");
            updateInventoryTable();
            updateStatisticsTab();
            QMessageBox::information(
                this,
                isUk ? "Успіх" : "Success",
                isUk ? "Інгредієнт успішно додано!" : "Ingredient added successfully!"
            );
        }
    }
}