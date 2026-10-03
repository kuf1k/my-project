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
# 🥗 NutriMesh — Kitchen Assistant
NutriMesh — це проєкт, створений для розумного обліку продуктів, а також автоматичного підбору рецептів на основі наявних інгредієнтів.

Головна мета NutriMesh — допомогти раціонально використовувати продукти, мінімізувати харчові відходи та спростити щоденне планування харчування й покупок.


## ✨ Реалізований функціонал

- **Облік інгрідієнтів**: Облік твердих (у грамах) та рідких (у мілілітрах) інгредієнтів.
- **Контроль термінів придатності**: Фіксація дати придатності для кожного продукту (`YYYY-MM-DD`).
- **Підбір та фільтрація рецептів**: Перевірка наявності продуктів для рецепта та фільтрація рецептів за калорійністю.
- **Експорт звітів**: Збереження стану інвентарю у текстовий файл.
- **Інтерактивний CLI**: Консольне меню з захистом від некоректного вводу даних.
- **Модульне тестування**: Контроль роботи класів завдяки модульним тестам на GoogleTest.

---

## 🛠️ Технології

* **Мова програмування:** С++ (C++20)
* **Система збірки:** CMake
* **Фреймворк тестування:** GoogleTest

---
