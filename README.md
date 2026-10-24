# Репозиторій створений для навчального проєкту, в якому будуть виконані вимоги до лабораторних робіт.
Виконує: Благодір Владислав Володимирович.<br>
Група: ІПС-22.
## 📋 Виконання вимог Лабораторної роботи №1
* **Класи та типи (12):** Реалізовано 11 класів : (`Ingredient`, `SolidIngredient`, `LiquidIngredient`, `Inventory`, `Recipe`, `RecipeRequirement`, `RecipeMatcher`, `RecipeRepository`, `DateUtils`, `InventoryExporter`, `FileExporter`, `DataFilter<T>`) та 1 структуру (`RecipeRequirement`).
* **Поля класів (15):** 
  * `Ingredient`: `name` , `amount` , `expirationDate` .
  * `Inventory`: `items`.
  * `RecipeRequirement` (структура): `name`, `requiredAmount`, `unit`.
  * `Recipe`: `title`, `prepTimeMinutes`, `calories`, `instructions`, `requirements`.
  * `RecipeRepository`: `recipes`.
  * `InventoryExporter`: `reportTitle`.
  * `FileExporter`: `filePath`.
* **Нетривіальні методи/функції (30):**
   * *Управління запасами (`Inventory`):* `addIngredient`, `printInventory`, `removeExpired`, `findByName`, `printExpiringSoon`, `printStatistics`, `getIngredientAmount`.
    * *Логіка інгредієнтів (`Ingredient`, `SolidIngredient`, `LiquidIngredient`):* `isExpired`, `getFreshnessStatus`, поліморфні `printInfo` (2 шт.) та `getUnit` (2 шт.).
  * *Обробка дат (`DateUtils`):* `getDaysUntil`, `calculateStatus`, `getStatusLabel`, `isValidDate`.
  * *Рецепти (`RecipeMatcher`, `RecipeRepository`, `Recipe`):* `canPrepare`, `checkRecipeAvailability`, `addRequirement`, `printAllRecipes`, `getCookableRecipes`.
  * *Експорт та фільтрація (`FileExporter`, `DataFilter<T>`):* `exportData`, `filterBy`, `contains`.
  * *CLI (`main.cpp`):* `displayMenu`, `readAmount`, `readDate`, `enterInfo`, `runExport`.
* **Ієрархії успадкування (2):**
  1. `Ingredient` (абстрактний базовий) ➔ `SolidIngredient`, `LiquidIngredient` (3 класи).
  2. `InventoryExporter` (абстрактний базовий) ➔ `FileExporter` (2 класи).
* **Незалежні випадки поліморфізму (3):**
  * **Динамічний №1:** Робота з колекцією `std::vector<std::unique_ptr<Ingredient>>` у класі `Inventory` та виклик віртуальних методів `printInfo()` й `getUnit()`.
  * **Динамічний №2:** Клієнтська функція `runExport()` у `main.cpp` працює через посилання на абстрактний клас `InventoryExporter&`, не прив'язуючись до конкретної реалізації.
  * **Статичний (Шаблон):** Реалізовано та використано шаблонний клас `DataFilter<T>` для фільтрації даних.
 
*Додатково*
* **Unit Tests:** Модульне тестування реалізовано за допомогою фреймворку **GoogleTest**:
   * `IngredientTest` , `InventoryTest` - перевірка логіки інвентарю.
   * `DateUtilsTest` - перевірка роботи з датами.
   * `RecipeTest`, `RecipeMatcherTest` , `RecipeRepositoryTest` - перевірка обробки рецептів.
   * `FileExporterTest` - перевірка експорту у файл.
   * `DataFilterTest` - перевірка шаблонної фільтрації.
 ## 📋 Виконання вимог Лабораторної роботи №2
* **Вікна/екрани/форми (7):** Реалізовано 3 основні класи вікон : 
  * `MainWindow` (Головне вікно, яке включає 4 екрани: `Pantry & Fridge`, `What Can I Cook?`, `Recipe Book`, `Shopping List`).
  * `AddIngredientDialog` (Модальне вікно для додавання інгредієнтів).
  * `AddRecipeDialog` (Модальне вікно-конструктор власних рецептів).
* **Елементи керування (31):**
  * *Кнопки (`QPushButton`, 15):* `addIngredientButton`, `removeExpiredButton`, `exportInventoryButton`, `refreshButton`, `findRecipesButton`, `cookRecipeButton`, `addRecipeButton`, `generateShoppingListButton`, `exportShoppingListButton`, `addButton`, `cancelButton` (у двох діалогах), `addReqButton`, `removeReqButton`, `saveButton`.
  * *Випадаючі списки (`QComboBox`, 5):* `matcherSortComboBox`, `recipesSortComboBox`, `shoppingRecipeComboBox`, `typeComboBox`, `reqUnitComboBox`.
  * *Поля введення тексту (`QLineEdit`, `QTextEdit`, 6):* `cookableRecipeDetailsTextEdit`, `recipeDetailsTextEdit`, `nameLineEdit`, `titleLineEdit`, `instructionsTextEdit`, `reqNameEdit`.
  * *Числові поля (`QSpinBox`, `QDoubleSpinBox`, 4):* `amountSpinBox`, `prepTimeSpinBox`, `caloriesSpinBox`, `reqAmountSpinBox`.
  * *Поля вибору дати (`QDateEdit`, 1):* `dateEdit`.
* **Елементи-контейнери (6):**
  * `inventoryTable` (`QTableWidget`) — таблиця продуктів у холодильнику.
  * `shoppingTableWidget` (`QTableWidget`) — таблиця для списку покупок.
  * `requirementsTable` (`QTableWidget`) — таблиця інгредієнтів при створенні рецепта.
  * `cookableRecipesListWidget` (`QListWidget`) — список рецептів, доступних для приготування.
  * `recipesListWidget` (`QListWidget`) — список усіх збережених рецептів.
  * `tabWidget` (`QTabWidget`) — контейнер вкладок навігації головного вікна.
* **Обробники подій (сигнали та слоти, 18):** 
  * *Взаємодія з інвентарем (`MainWindow`, `AddIngredientDialog`, 6):* `onRefreshInventoryClicked`, `onAddIngredientClicked`, `onRemoveExpiredClicked`, `onExportInventoryClicked`, `onTypeChanged`, `onAddClicked`.
  * *Робота з рецептами та списком покупок (`MainWindow`, 9):* `onRecipeSelectionChanged`, `onFindCookableRecipesClicked`, `onCookableRecipeSelectionChanged`, `onGenerateShoppingListClicked`, `onExportShoppingListClicked`, `onCookRecipeClicked`, `onRecipesSortChanged`, `onMatcherSortChanged`, `onAddRecipeClicked`.
  * *Конструктор рецептів (`AddRecipeDialog`, 3):* `onAddRequirementClicked`, `onRemoveRequirementClicked`, `onSaveClicked`.
# 🥗 NutriMesh — Kitchen Assistant
**NutriMesh** — це проєкт, створений для розумного обліку продуктів, автоматичного підбору рецептів на основі наявних інгредієнтів, створення власних страв та генерації списків покупок.

**Головна мета NutriMesh** — допомогти раціонально використовувати продукти, мінімізувати харчові відходи та спростити щоденне планування харчування й покупок.


## ✨ Реалізований функціонал

- **Графічний інтерфейс (GUI):** Інтуїтивний інтерфейс, що замінив консольне меню.
- **Розумний облік інгрідієнтів:**
  * Перегляд, додавання та видалення (протермінованих) інгредієнтів.
  * Кольорова індикація інгредієнтів за терміном придатності (свіжі 🟢, скоро зіпсуються 🟡, протерміновані 🔴).
- **Книга рецептів:** Зручний каталог усіх збережених рецептів із можливістю сортування **за назвою, часом приготування та калорійністю**.
- **Підбір страв:** Автоматичний пошук рецептів, які можна приготувати, базуючись на вмісті вашого холодильника.
- **Конструктор власних рецептів:** Можливість створювати власні рецепти, додавати до них список необхідних інгредієнтів та зберігати у базу рецептів.
- **Генератор списку покупок:** Автоматичний розрахунок точної кількості інгредієнтів, яких не вистачає для приготування обраної страви.
- **Експорт звітів та серіалізація:** Збереження стану програми (інвентарю та бази рецептів) у `JSON` файли, а також експорт звітів інвентарю і списку покупок у `TXT` файли.
- **Модульне тестування**: Контроль роботи класів завдяки модульним тестам на GoogleTest.

---

## 🛠️ Технології

* **Мова програмування:** С++ (C++20)
* **Система збірки:** CMake
* **Фреймворк тестування:** GoogleTest
* **Графічний фреймворк:** Qt 6 (Qt Widgets)
* **Робота з даними:** nlohmann/json (серіалізація/десеріалізація)

---
